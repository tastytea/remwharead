/* globals browser */

let taburl = "";

const txttags = document.getElementById("txttags");
const chkarchive = document.getElementById("chkarchive");
const btnadd = document.getElementById("btnadd");
const msgstatus = document.getElementById("msgstatus");
const msgerror = document.getElementById("msgerror");


function set_taburl(tabs)       // Set taburl to URL of current tab.
{
    const tab = tabs[0];
    taburl = '\'' + tab.url + '\'';
}

function get_tags()             // get tags from text input.
{
    const tags = txttags.value;
    if (tags != "")
    {
        return "-t '" + tags + "' ";
    }
    return "";
}

function read_options()
{
    const item = browser.storage.sync.get('archive');
    item.then((res) =>
              {
                  if (res.archive === false)
                  {
                      chkarchive.checked = false;
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
    const sending = browser.runtime.sendNativeMessage("remwharead", args);
    sending.then(onResponse, onError);
}

function add()
{
    let archive = "";
    if (chkarchive.checked === false)
    {
        archive = "--no-archive ";
    }
    const args = get_tags() + archive + taburl;
    console.log(args);
    launch(args);
}

read_options();

// Call set_taburl() with current tab.
browser.tabs.query({currentWindow: true, active: true}).then(set_taburl);

btnadd.addEventListener("click", add);

txttags.addEventListener(       // Call launch() if enter is hit in text input.
    "keyup", event =>
        {
            if(event.key !== "Enter")
            {
                return;
            }
            add();
            event.preventDefault();
        });
