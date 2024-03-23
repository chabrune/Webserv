let cardnumber = document.forms["register_form"]["cardnumber"].value

let numericPattern = /^[0-9]+$/;

if (numericPattern.test(cardnumber))
    console.log("ui");
else
    console.log("Non");


document.forms["register_form"]["cardnumber"].addEventListener('keypress', numericFilter);
document.forms["register_form"]["cardname"].addEventListener('keypress', alphaFilter);
document.forms["register_form"]["cvvnumber"].addEventListener('keypress', numericFilter);


function alphaFilter(event) {
    const keyChar = getKey(event)
    if (isNumeric(keyChar))
        event.preventDefault();
}

function numericFilter(event) {
    const keyChar = getKey(event)
    if (!isNumeric(keyChar))
        event.preventDefault();
}


function getKey(event) {
    const keyCode = event.keyCode || event.which;
    // Allow backspace, delete, tab, and arrow keys
    if (keyCode === 8 || keyCode === 46 || keyCode === 9 || (keyCode >= 37 && keyCode <= 40))
        return true;
    // Convert the key code to its corresponding character
    return String.fromCharCode(keyCode);
}

function isNumeric(keyChar) {
    return /\d/.test(keyChar)
}