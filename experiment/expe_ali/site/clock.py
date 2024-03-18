#!/usr/bin/env python3

# Set the content type to HTML
print("Content-Type: text/html\n")

# HTML boilerplate
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Real-Time Clock</title>")
print("</head>")
print("<body>")

# Display the current time using JavaScript
print("""
<script>
function updateClock() {
    var now = new Date();
    var hours = String(now.getHours()).padStart(2, '0');
    var minutes = String(now.getMinutes()).padStart(2, '0');
    var seconds = String(now.getSeconds()).padStart(2, '0');
    var timeString = hours + ':' + minutes + ':' + seconds;
    document.getElementById('clock').textContent = timeString;
}

// Update the clock every second
setInterval(updateClock, 1000);

// Initial call to updateClock to start the clock immediately
updateClock();
</script>
""")

# Display the clock placeholder
print("<h1>Real-Time Clock</h1>")
print("<p>The current time is: <span id='clock'></span></p>")

# HTML closing tags
print("</body>")
print("</html>")