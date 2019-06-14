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
    const item = browser.storage.sync.get('archive');
    item.then((res) =>
              {
                  if (res.archive === false)
                  {
                      document.querySelector("#archive").checked = false;
                  }
              });
}

function info_commands(commands)
{
    commands.forEach(
        function(command)
        {
            const element = document.querySelector("#shortcuts");
            const para = document.createElement("p");

            const text = document.createElement("strong");
            text.appendChild(document.createTextNode(command.shortcut + ": "));
            para.appendChild(text);
            para.appendChild(document.createTextNode(command.description));

            element.appendChild(para);
        });
}

document.addEventListener('DOMContentLoaded', restore_options);
document.querySelector("#archive").addEventListener("change", save_options);

const get_commands = browser.commands.getAll();
get_commands.then(info_commands);
