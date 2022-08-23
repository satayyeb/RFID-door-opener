function prepareLearn() {
    clearMain();
    addText('h1', 'Learn a new card');
    addText('h4', 'This operation makes the device open the door when detects this card.');
    addText('p', "Hold your new card behind the device and then click on the button below.");
    addButton("Learn", "put the new card then click this button", "learn()");
}

function learn() {
    //TODO: this
    alert("hello world!");
}

function prepareReset() {
    clearMain();
    addText('h1', 'Reset a card');
    addText('h4', 'This operation resets your card and removes it from the system.');
    addText('p', "Hold your card behind the device and then click on the button below.");
    addButton("Reset", "put the card then click this button", "reset()");
}


function reset() {
    alert("hello world!");
}

function prepareDelete() {
    clearMain();
    load('select-card-form.html', 'main');
}


function showAllCards() {
    clearMain();
    //TODO: this
    document.getElementById('main').innerHTML =
        "<h1>Cards List</h1>\
    <ul style='text-align: left;'>\
        <li>card number 1 - user: ali</li>\
        <li>tag  number 2 - user: ali</li>\
        <li>card number 3 - user: wasim</li>\
        <li>card number 4 - user: wasan</li>\
        <li>card number 5 - user: zahrah</li>\
        <li>card number 6 - user: nassim</li>\
        <li>This is just a template. implement this.</li>\
    </ul>"
}

function prepareDeleteAll() {
    clearMain();
    addText('h1', 'Delete all cards');
    addText('h4', 'This operation deletes any card information from the memory. No card will open the door after this operation. If you want to add (learn) your cards again, you must change the sector in RFID menu before. beacuse the current sector on cards is written and you should set the sector settings to a free sector.');
    addText('p', "If you want to remove all cards from memory click the button below.");
    addButton("Delete all", "Take care! This erase the memory!", "deleteAll()");
}

function deleteAll() {
    if (confirm('Are you sure you want to delete all cards from memory?')) {
        //TODO: this
    }
}

function prepareChangeSector() {
    clearMain();
    load('form.html', 'main');
}

