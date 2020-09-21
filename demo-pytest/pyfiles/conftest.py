#File consisting of fixtures that could be shared between several tests
import pytest
import json
import test_calc as test_calc
import os


def pytest_sessionfinish(session, exitstatus):
    data = test_calc.pytest.json_metadata
    with open('result.json', 'w') as f:
        json.dump(data, f, indent=4)
