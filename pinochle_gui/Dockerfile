# Stage 1: Build the React Frontend
FROM node:18-alpine as build-stage
WORKDIR /app/frontend
COPY frontend/package*.json ./
RUN npm install
COPY frontend/ ./
RUN npm run build

# Stage 2: Build the FastAPI Backend
FROM python:3.11-slim
WORKDIR /app

# Install dependencies
COPY backend/requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

# Copy backend code
COPY backend/ ./backend/

# Copy frontend build to the expected location
COPY --from=build-stage /app/frontend/build ./frontend/build

# Set the working directory to the backend so imports work correctly
WORKDIR /app/backend

EXPOSE 8000

# Run from the backend directory
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
