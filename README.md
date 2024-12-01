## Command help text
```bash
passman -h
```
### Points of interest
- explain that commands follow a noun-verb structure, e.g. `vault create`, `entry add`, etc.

## Create a vault
```bash
passman vault create test-vault
```
### Points of interest:
- mismatching master passwords
- in the interest of time, leave the password an empty string

## Add entries
```bash
# prompt password
passman entry add --user user1 --url domain1.com -p --notes "hello from myEntry1!" test-vault myEntry1
```
```bash
# generate password
passman entry add --user user2 --url domain2.com -g --notes "hello from myEntry2!" test-vault myEntry2
```
```bash
# diceware password
passman entry add --user user3 --url domain3.com -gw --notes "hello from myEntry3!" test-vault myEntry3
```
```bash
# will fail, conflicting flags
passman entry add --user user4 --url domain4.com -gwy --notes "hello from myEntry4!" test-vault myEntry4
```
### Points of interest
- show the vault file with something like `cat test-vault` or through the gui
- `generate` and `diceware` work as standalone commands too

## List entries
```bash
# should print titles of entries just created
passman entry ls test-vault
```

## Edit entry
```bash
# show entry before edit
passman entry show -s test-vault myEntry1
```
```bash
# generate new password
passman entry edit -g test-vault myEntry1
```
```bash
# show entry after edit
passman entry show -s test-vault myEntry1
```
### Points of interest
- mention that all entry fields are editable via same flags as `entry add`

## Remove entry
```bash
# remove entry
passman entry rm test-vault myEntry1
```
```bash
# show that the entry is deleted
passman entry ls test-vault
```
```bash
# this wil now fail
passman entry show -s test-vault myEntry1
```

## Clip entry
```bash
# show the diceware entry
passman entry show -s test-vault myEntry3
```
```bash
# clip the password and paste it somewhere
passman entry clip test-vault myEntry3
```
```bash
# demo the cleared clipboard with this command
# alternatively, show a right-click+paste is blank in a textfile
xclip -o -selection clipboard
```