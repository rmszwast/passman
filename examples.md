## Command help text
```bash
passman -h
```

## Create a vault
```bash
passman vault create test-vault
```

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
