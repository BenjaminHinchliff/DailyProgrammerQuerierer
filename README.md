# DailyProgrammerQuerierer
A little tool to pull challenges off [/r/dailyprogrammer](https://www.reddit.com/r/dailyprogrammer/) and save them into local markdown files

## Help
```
Options:
  -h [ --help ]                         print help message to stdout
  -n [ --number ] arg                   number of the challenges to download or
                                        range or all) (positional)
  -d [ --difficulties ] arg (=easy intermediate hard)
                                        the difficulties of the challenges to
                                        download (positional after number)
  -o [ --output ] arg (=.)              the output path for the files
                                        (optional)
```

## Examples
Get challenge #380 with easy difficulty:
```dp 380 easy``` 
Get challenges in range 370-380 with easy or hard difficulties:
```dp 370-380 easy hard```
Get challenges of any diffuculty with number of 380:
```dp 380```
Get all easy challenges:
```dp all easy```
All challenges: *(not recommended - search function on the reddit api is slightly inaccurate and many could be wrong)* 
```dp all```
