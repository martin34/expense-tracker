```
cd data/pdf
ls | xargs -I% pdftotext -layout %
cd ..
mv pdf/*.txt txt
```


### Next steps:

1. Generate report, which can be easily visualized in excel like tool :heavy_check_mark:
1. Import n26 data :heavy_check_mark:
1. Improve filter logic (extend, configurable with filter config files) :heavy_check_mark:
1. Improve command line interface to something like `bazel run //tracker:bin --config=asan -- --load_project_from /home/martin/repo/finance/foo --save_project_to /home/martin/repo/finance/foo` :heavy_check_mark:
1. Fix bug in paring rba data. IBAN is not parsed correctly and spaces are not fully removed from reason :heavy_check_mark:
1. Add filters for most elements in list e. g. introduce patner_name filter
1. Add possibility to categories last 20 % by hand (extend user interface)