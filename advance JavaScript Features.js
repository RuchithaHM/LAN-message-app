function sendMessage() {

    let input = document.getElementById("messageInput");
    let chatBox = document.getElementById("chatBox");

    if(input.value.trim() === "") return;

    let time = new Date().toLocaleTimeString();

    let message = document.createElement("div");

    message.classList.add("message");
    message.classList.add("right");

    message.innerHTML = `${input.value}<br><small>${time}</small>`;

    chatBox.appendChild(message);

    notificationSound();

    chatBox.scrollTop = chatBox.scrollHeight;

    input.value = "";
}

function notificationSound() {
    let audio = new Audio();
    audio.src = "notification.mp3";
    audio.play();
}
