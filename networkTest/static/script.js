const { useReducer } = require("react");

const API_URL = "/users";

const userList = document.getElementById("user-list");
const addBtn = document.getElementById("add-btn");
const nameInput = document.getElementById("name");
const messageBox = document.getElementById("message");

//render 

async function loadUsers() {
    const res = await fetch(API_URL);
    const users = await res.json();

    userList.innerHTML = ""; // clears list
    users.forEach(u => addUserToDOM(u));
}

function addUserToDOM(user) {
    const item = document.createElement("button");
    item.textContent = user.name;

    const delBtn = document.createElement("button");

    delBtn.textContent = "Delete";
    delBrn.onclick = async () => {
        await fetch (`${API_URL}/${user.id}`, {method : "DELETE"} );
        item.remove();
        showMessage(`Deleted ${user.name}`);
    };

    item.appendChild(delBtn);
    userList.appendChild(item);
}

async function handleAddUser() {
    const name = nameInput.value.trim();
    if (!name) return showMessage("ENTER A NAME DUMBASS", true);

    const res = await fetch(API_URL, {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify({name})
    });
    if (res.ok) {
        const newUser = await res.json();
        addUserToDOM(newUser);
        nameInput.value="";
        showMessage(`Added ${user.name}`);
    } else {
        showMessage("Failed to add user", true);
    }
}

function showMessage(text, isError = false) {
    messageBox.textContent = text;
    messageBox.style.color = isError ? "red" : "green";
    setTimeout(() => (messageBox.textContent = ""), 2500);
}

addBtn.addEventListener("click", handleAddUser);
loadUsers();