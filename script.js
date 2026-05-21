function sendMessage() {

    let input = document.getElementById("messageInput");
    let chatBox = document.getElementById("chatBox");

    if(input.value.trim() === "") {
        return;
    }

    let message = document.createElement("div");

    message.classList.add("message");
    message.classList.add("right");

    message.innerText = input.value;

    chatBox.appendChild(message);

    chatBox.scrollTop = chatBox.scrollHeight;

    input.value = "";
}
