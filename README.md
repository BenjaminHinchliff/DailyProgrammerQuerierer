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
`dp 380 easy` get challenge #380 with easy difficulty
`dp 370-380 easy hard` get challenges in range 370-380 with easy or hard difficulties
`dp 380` get challenges of any diffuculty with number of 380
`dp all easy` all easy challenges
`dp all` all challenges *(not recommended - search function on the reddit api is slightly inaccurate and many could be wrong)* 
