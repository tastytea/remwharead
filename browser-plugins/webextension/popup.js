var taburl = "";
var archive = "";

function set_taburl(tabs)       // Set taburl to URL of current tab.
{
    let tab = tabs[0];
    taburl = tab.url;
}

function get_tags()             // get tags from text input.
{
    let tags = txttags.value;
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
    msgstatus.textContent = "";

    if (response == "Command successful.")
    {
        window.close();
    }
    else
    {
        msgerror.textContent = response;
    }

}

function onError(error) {
    console.log(`Error: ${error}`);
    msgerror.textContent = "Could not launch remwharead.";
    msgstatus.textContent = "";
}

function launch(args)           // Launch wrapper and send tags + URL to stdin.
{
    msgstatus.textContent = "Launching remwharead…";
    msgerror.textContent = "";
    console.log("Sending: " + args + " to remwharead");
    var sending = browser.runtime.sendNativeMessage("remwharead", args);
    sending.then(onResponse, onError);
}

function add()
{
    var arguments = get_tags() + archive + taburl;
    launch(arguments);
}

read_options();

// Call set_taburl() with current tab.
browser.tabs.query({currentWindow: true, active: true}).then(set_taburl);

btnadd.addEventListener("click", launch);

txttags.addEventListener(       // Call launch() if enter is hit in text input.
    "keyup", event =>
        {
            if(event.key !== "Enter")
            {
                return;
            }
            launch();
            event.preventDefault();
        });
