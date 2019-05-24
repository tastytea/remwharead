function save_options(e)
{
    browser.storage.sync.set(
        {
            archive: document.querySelector("#archive").checked
        });
    e.preventDefault();
}

function restore_options()
{
    var item = browser.storage.sync.get('archive');
    item.then((res) =>
              {
                  document.querySelector("#archive").checked = res.archive;
              });
}

function info_commands(commands)
{
    commands.forEach(
        function(command)
        {
            var element = document.querySelector("#shortcuts");
            var para = document.createElement("p");

            var text = document.createElement("strong");
            text.appendChild(document.createTextNode(command.shortcut + ": "));
            para.appendChild(text);
            para.appendChild(document.createTextNode(command.description));

            element.appendChild(para);
        });
}

document.addEventListener('DOMContentLoaded', restore_options);
document.querySelector("#archive").addEventListener("change", save_options);

var get_commands = browser.commands.getAll();
get_commands.then(info_commands);
