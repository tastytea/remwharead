var taburl = "";
var archive = "";

function set_taburl(tabs)       // Set taburl to URL of current tab.
{
    let tab = tabs[0];
    taburl = tab.url;
}

function get_tags()             // get tags from text input.
{
    let tags = document.getElementById("tags").value;
    if (tags != "")
    {
        return "-t '" + tags + "' ";
    }
    return "";
}

function read_options()
{
    var item = browser.storage.sync.get('archive');
    item.then((res) =>
              {
                  if (res.archive === false)
                  {
                      archive = "--no-archive ";
                  }
              });
}

function onResponse(response) {
    console.log("Received: " + response);
    document.getElementById("status").textContent = "";

    if (response == "Command successful.")
    {
        window.close();
    }
    else
    {
        document.getElementById("error").textContent = response;
    }

}

function onError(error) {
    console.log(`Error: ${error}`);
    document.getElementById("error").textContent = "Could not launch remwharead.";
    document.getElementById("status").textContent = "";
}

function launch()               // Launch wrapper and send tags + URL to stdin.
{
    document.getElementById("status").textContent = "Launching remwharead…";
    document.getElementById("error").textContent = "";
    var arguments = get_tags() + archive + taburl;
    console.log("Sending: " + arguments + " to remwharead");
    var sending = browser.runtime.sendNativeMessage("remwharead", arguments);
    sending.then(onResponse, onError);
}

read_options();

// Call set_taburl() with current tab.
browser.tabs.query({currentWindow: true, active: true}).then(set_taburl);

button.addEventListener("click", launch); // Call launch() if button is clicked.

// Call launch if enter is hit in text input.
document.querySelector("#tags").addEventListener(
    "keyup", event =>
        {
            if(event.key !== "Enter")
            {
                return;
            }
            launch();
            event.preventDefault();
        });
