import RDAnalysis_test

if __name__ == "__main__":
    ts = RDAnalysis_test.Test_RDA()

    print('\nParising source.txt:')
    ts.test_run_source()

    print('\nParising source1.txt:')
    ts.test_run_source_1()

    print('\nParsing source2.txt:')
    ts.test_run_source_2()

    print('\nParsing source3.txt:')
    ts.test_run_source_3()