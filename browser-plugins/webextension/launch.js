var taburl;

function onResponse(response)
{
    console.log(`Received ${response}`);
}

function onError(error)
{
    console.log(`Error: ${error}`);
}

function set_taburl(tabs)
{
    let tab = tabs[0];
    taburl = tab.url;
}

function launch()
{
    let tags = document.getElementById("tags").value;
    var sending = browser.runtime.sendNativeMessage("remwharead", "-t " + tags + " " + taburl);
    sending.then(onResponse, onError);
}

browser.tabs.query({currentWindow: true, active: true}).then(set_taburl);
add_uri.addEventListener("submit", launch);
