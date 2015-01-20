How to run step B (a.k.a. 2+3)
====

For MC

    cd src/LatinoTrees/AnalysisStep/test/
    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/40F65A6C-2209-E411-9E44-003048CDCEC2.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_vbfhww.root
    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/40F65A6C-2209-E411-9E44-003048CDCEC2.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_vbfhww.root               doNoFilter=True
    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/40F65A6C-2209-E411-9E44-003048CDCEC2.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_vbfhww.root               doNoFilter=True   doEleIsoId=True

    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/00CAA728-D626-E411-9112-00215AD4D6E2.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_tt_normalmix.root       doNoFilter=True
    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/00800BE3-E826-E411-AD01-20CF3019DEE9.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_tt_premix.root          doNoFilter=True

    Phys14
    cmsRun stepB.py print inputFiles=file:/tmp/amassiro/C667E84D-9D18-E411-99D8-02163E00ECE6.root        label=WW id=123456789  scale=1 outputFile=/tmp/amassiro/stepB_latinosYieldSkim_MC_ggHww.root          doNoFilter=True





Create final tree
====

    python cmssw2latino.py   /tmp/amassiro/stepB_latinosYieldSkim_MC_vbfhww.root

    python cmssw2latino.py   /tmp/amassiro/stepB_latinosYieldSkim_MC_tt_normalmix.root
    python cmssw2latino.py   /tmp/amassiro/stepB_latinosYieldSkim_MC_tt_premix.root

    python cmssw2latino.py   /tmp/amassiro/stepB_latinosYieldSkim_MC_ggHww.root

