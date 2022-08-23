function openDoor() {
    if (confirm("Are you sure you want to open the door now?")) {
        //TODO: open door
        window.location.href = '/open';
        const node = document.getElementById("main");
        const child = document.createElement("h4");
        child.appendChild(document.createTextNode("The door opened successfully."));
        node.appendChild(child);
        setTimeout(function () { child.remove(); }, 2000);
    }
}


function troubleshoot() {

}