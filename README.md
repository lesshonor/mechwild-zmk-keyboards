# MechWild's Keyboard Module Repo (WIP)

(If you're reading this, fork at your own risk. There *will* be rebasing until this repo gets closer to "done".)

## Instructions

Add this repository to your existing `config/west.yml` as shown below. (No idea what that means? [Check this repository instead!](https://github.com/lesshonor/mechwild-zmk-config))

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    # Add the base GitHub URL as a remote.
    - name: mechwild-temporary # You can name this whatever you like.
      url-base: https://github.com/lesshonor
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    # Add the name of the repository as a project.
    - name: mechwild-zmk-keyboards
      remote: mechwild-temporary # Note, this has to match the above "remote".
      revision: main # This is the name of the branch you want to use.
  self:
    path: config
```

After doing so, copy any keymaps you may need out of the appropriate board and/or shield folders and paste them into your `config` folder for later modification. Don't forget to edit your `build.yaml`.

---

## TODO

- What's in this repo, what isn't, and perhaps why
- What features on which keyboards don't work or have caveats in ZMK
- How best to organize? Some info like the MurphPad VCC/RAW bypass probably justifies its own README.
