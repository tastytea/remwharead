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

document.addEventListener('DOMContentLoaded', restore_options);
document.querySelector("#archive").addEventListener("change", save_options);
