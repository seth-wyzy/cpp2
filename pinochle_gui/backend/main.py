from fastapi import FastAPI, HTTPException, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from pydantic import BaseModel
from typing import List, Optional, Dict
import json
import os
from game_logic import Game

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

class ConnectionManager:
    def __init__(self):
        self.active_connections: List[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def broadcast(self, message: str):
        for connection in self.active_connections:
            await connection.send_text(message)

manager = ConnectionManager()
game = Game()

async def broadcast_state():
    await manager.broadcast(json.dumps(game.get_state()))

class JoinRequest(BaseModel):
    seat_index: int
    name: str

class BidRequest(BaseModel):
    seat_index: int
    amount: int

class TrumpRequest(BaseModel):
    suit: int

class PlayRequest(BaseModel):
    seat_index: int
    card_index: int

class MeldConfirmRequest(BaseModel):
    seat_index: int
    selected_indices: List[int]

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)
    try:
        # Send initial state
        await websocket.send_text(json.dumps(game.get_state()))
        while True:
            await websocket.receive_text() # Keep alive
    except WebSocketDisconnect:
        manager.disconnect(websocket)

@app.post("/lobby/join")
async def join_seat(req: JoinRequest):
    success = game.join_seat(req.seat_index, req.name)
    if not success:
        raise HTTPException(status_code=400, detail="Seat occupied or invalid")
    await broadcast_state()
    return game.get_state()

@app.post("/game/start")
async def start_game():
    game.start_game()
    await broadcast_state()
    return game.get_state()

@app.post("/game/new")
async def new_game():
    game.__init__() # Reset everything
    await broadcast_state()
    return game.get_state()

@app.post("/game/next_round")
async def next_round():
    game.start_next_round()
    await broadcast_state()
    return game.get_state()

@app.get("/game/state")
def get_state():
    return game.get_state()

@app.post("/game/bid")
async def place_bid(req: BidRequest):
    if game.phase != "bidding" or game.current_bidder != req.seat_index:
        raise HTTPException(status_code=400, detail="Not your turn to bid or wrong phase")
    game.handle_bid(req.seat_index, req.amount)
    # AI loop is already inside handle_bid in my latest game_logic.py
    await broadcast_state()
    return game.get_state()

@app.post("/game/trump")
async def select_trump(req: TrumpRequest):
    if game.phase != "trump_selection":
        raise HTTPException(status_code=400, detail="Wrong phase for trump selection")
    game.select_trump(req.suit)
    await broadcast_state()
    return game.get_state()

@app.post("/game/meld/confirm")
async def confirm_meld(req: MeldConfirmRequest):
    game.confirm_user_meld(req.seat_index, req.selected_indices)
    await broadcast_state()
    return game.get_state()

@app.post("/game/start_tricks")
async def start_tricks():
    if game.phase != "meld_display":
         raise HTTPException(status_code=400, detail="Wrong phase to start tricks")
    game.start_tricks()
    await broadcast_state()
    return game.get_state()

@app.post("/game/ai_play")
async def ai_play():
    played = game.ai_play_one()
    if played:
        await broadcast_state()
    return {"played": played, "state": game.get_state()}

@app.post("/game/play")
async def play_card(req: PlayRequest):
    curr_p = (game.trick_leader + len(game.current_trick)) % 4
    if game.phase != "trick_taking" or curr_p != req.seat_index:
        raise HTTPException(status_code=400, detail="Not your turn to play or wrong phase")
    
    if req.card_index < 0 or req.card_index >= len(game.hands[req.seat_index]):
        raise HTTPException(status_code=400, detail="Invalid card index")
        
    try:
        game.play_card(req.seat_index, req.card_index)
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
        
    await broadcast_state()
    return game.get_state()

@app.post("/game/evaluate")
async def evaluate():
    if game.phase != "trick_taking" or len(game.current_trick) != 4:
         raise HTTPException(status_code=400, detail="Trick not finished")
    game.evaluate_trick()
    await broadcast_state()
    return game.get_state()

# Serve static files from the React build directory
# We check multiple locations to be robust for different deployment environments
possible_frontend_dirs = [
    os.path.join(os.path.dirname(__file__), "../frontend/build"),
    os.path.join(os.path.dirname(__file__), "static"),
    "/app/frontend/build",
]

frontend_dir = None
for d in possible_frontend_dirs:
    if os.path.exists(d):
        frontend_dir = d
        break

if frontend_dir:
    print(f"Serving frontend from: {frontend_dir}")
    app.mount("/", StaticFiles(directory=frontend_dir, html=True), name="static")
else:
    print("Warning: Frontend directory not found. API-only mode.")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
