from flask import Flask, jsonify, request, render_template
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)

app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///users.db"
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

# Initialize the database
db = SQLAlchemy(app)

# Define user model
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)

    def to_dict(self):
        return {"id": self.id, "name": self.name}

# Create tables
with app.app_context():
    db.create_all()

@app.route("/")
def index():
    return render_template("index.html")

# GET all users
@app.route("/users", methods=["GET"])
def get_users():
    users = User.query.all()
    return jsonify([u.to_dict() for u in users])

# GET user by ID
@app.route("/users/<int:user_id>", methods=["GET"])
def get_user(user_id):
    user = User.query.get(user_id)
    if user:
        return jsonify(user.to_dict())
    return jsonify({"error": "User not found"}), 404

# POST create new user
@app.route("/users", methods=["POST"])
def create_user():
    data = request.get_json()
    new_user = User(name=data["name"])
    db.session.add(new_user)
    db.session.commit()
    return jsonify(new_user.to_dict()), 201

# PUT update existing user
@app.route("/users/<int:user_id>", methods=["PUT"])
def update_user(user_id):
    user = User.query.get(user_id)
    if not user:
        return jsonify({"error": "User not found"}), 404
    data = request.get_json()
    user.name = data["name"]
    db.session.commit()
    return jsonify(user.to_dict())

# DELETE a user
@app.route("/users/<int:user_id>", methods=["DELETE"])
def delete_user(user_id):
    user = User.query.get(user_id)
    if not user:
        return jsonify({"error": "User not found"}), 404
    db.session.delete(user)
    db.session.commit()
    return jsonify({"message": "User deleted"}), 200

if __name__ == "__main__":
    app.run(debug=True)
