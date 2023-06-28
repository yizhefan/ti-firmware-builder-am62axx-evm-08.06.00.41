#!/usr/bin/env python
# Copyright (C) 2018 Texas Instruments Incorporated - http://www.ti.com/
# ALL RIGHTS RESERVED

'''
This script was used to update the test steps of each TIOVX test case.

To apply this to a different project, modify the query_fmt variable to
use the appropriate project name.
'''

from ti_atlassian import ti_qmetry as tiqm

def my_gen_func(start_at, max_res, jql):
    r = qinst.search_issues(jql,
        startAt=start_at,
        maxResults=max_res,
        fields="key",
        json_result=True)
    return r

def my_extractor(r):
        s = r['startAt']
        t = r['total']
        d = r['issues']
        return s, t, d, None

# Create a qmetry instance
qinst = tiqm()

# Query format
query_fmt='project = "TIOVX" AND type = "Test Case" and status = "Done"'

# Extracting all issues
r1 = qinst.ti_jira_iterate_all_issues(my_gen_func,
                                    my_extractor, 0, jql=query_fmt)

# Iterating over issues
for testcase in r1:
    t_key = testcase['key']

    res = qinst.qm_create_a_test_step(
        tc_key=t_key,
        step_summary="Follow Manual.ConformanceTests.PC on https://confluence.itg.ti.com/display/VisionAnalytics/TIOVX+Test+Plan",
        expected_result="Follow Manual.ConformanceTests.PC on https://confluence.itg.ti.com/display/VisionAnalytics/TIOVX+Test+Plan",
        test_data="N/A"
    )

# Format via !autopep8 -i -a %
# vim: et:ts=4
