function prepareLearn() {
    clearMain();
    addText('h1', 'Learn a new card');
    addText('h4', 'This operation makes the device open the door when detects this card.');
    addText('p', "Hold your new card behind the device and then click on the button below.");
    addButton("Learn", "put the new card then click this button", "learn()");
    addBlankTag('p', 'message');
}

function learn() {
    fetch('/learn')
        .then((response) => response.text())
        .then((text) => {
            document.getElementById('message').innerHTML = "<p>" + text + "</p>";
        })
}

function prepareReset() {
    clearMain();
    addText('h1', 'Reset a card');
    addText('h4', 'This operation resets your card and removes it from the system.');
    addText('p', "Hold your card behind the device and then click on the button below.");
    addButton("Reset", "put the card then click this button", "reset()");
    addBlankTag('p', 'message');
}


function reset() {
    fetch('/reset')
        .then((response) => response.text())
        .then((text) => {
            document.getElementById('message').innerHTML = "<p>" + text + "</p>";
        })
}

function prepareDelete() {
    clearMain();
    load('select-card-form.html', 'main');
    addBlankTag('p', 'message');
}


function showAllCards() {
    clearMain();
    load('get-cards', 'main');
}

function prepareDeleteAll() {
    clearMain();
    addText('h1', 'Delete all cards');
    addText('h4', 'This operation deletes any card information from the memory. No card will open the door after this operation. If you want to add (learn) your cards again, you must change the sector in RFID menu before. beacuse the current sector on cards is written and you should set the sector settings to a free sector.');
    addText('p', "If you want to remove all cards from memory click the button below.");
    addButton("Delete all", "Take care! This erase the memory!", "deleteAll()");
    addBlankTag('p', 'message');
}

function deleteAll() {
    if (confirm('Are you sure you want to delete all cards from memory?')) {
        if (confirm('TAKE CARE! This action deletes all cards from the system. Are you sure you want to delete all?')) {
            fetch('/delete-all')
                .then((response) => response.text())
                .then((text) => {
                    document.getElementById('message').innerHTML = "<p>" + text + "</p>";
                })
        }
    }
}


