function loadHeaderAndFooter(){
    load('header.html','header');
    load('footer.html','footer');
}

function load(file, id){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', file, true);
    xhr.onreadystatechange = function () {
        if (this.readyState !== 4) return;
        if (this.status !== 200) return; // or whatever error handling you want
        document.getElementById(id).innerHTML = this.responseText;
    };
    xhr.send();
}


function addButton(text, tooltip, func) {
    const newTag = document.createElement("input");
    newTag.setAttribute('type', 'button')
    newTag.setAttribute('class', 'button');
    newTag.setAttribute('value', text);
    newTag.setAttribute('title', tooltip);
    newTag.setAttribute('onclick', func);
    const element = document.getElementById("main");
    element.appendChild(newTag);
}

function addText(tag, text) {
    const newTag = document.createElement(tag);
    const textInTag = document.createTextNode(text);
    newTag.appendChild(textInTag);
    const element = document.getElementById("main");
    element.appendChild(newTag);
}

function clearMain() {
    const myNode = document.getElementById("main");
    while (myNode.firstChild)
        myNode.removeChild(myNode.lastChild);
}