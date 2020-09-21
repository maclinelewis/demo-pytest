# ADI Demos

This master project can be used to create multiple demo for ADI. Strictly Don't put any ADI IPs in this

Requirements:
-------------
Windows Host
python3
gcc compiler
pyserial	: pip install pyserial
pytest		: pip install pytest
1 pair of virtual COM port
Note: port names are hardcoded COM3(test_calc.py) and COM4(parser.py)

Setting Virtual COM port:
-------------------------
Step 1: Install virtual COM port using Null-model-emulator from 
"http://com0com.sourceforge.net/"

Step 2: Post installation, check in the option "Launch Setup" and click
	on Finish
Step 3: Once the UI for com0com is opened, expand the Virtual Port Pair1
	and make sure two ports (say COM3 and COM4) are present.
	If ports are not present click on "Add Pair" and on the
	text boxes appearing type the port names prefixed with COM
	(say COM3 and COM4), and enable the checkbox "use Ports class"
	for the pair. Click on Apply
Note down the names of both ports.

Description:
------------
The "demo_pytest" contains files for the demo testing implementation
using pytest and a mock of the target application

calc.c is the DUT or module under test. It has basic mathematical functions
like addition, subtraction, multiplication and division.

parser.c is the mock of the firmware application (main.c). This file uses
Win32 API's for connecting to serial port in windows. It initializes
a COM port(COM4) for reading, and continuously reads the string from the
port. It parses this string and decodes the test name and arguments, passes
the arguments to the module under test and executes the corresponding modules
to be tested, and writes the results to the COM port.

test_calc.py uses pytest testing framework. It had a total of 12 test
functions. The test_calc.py writes the testname and arguments in the
format "test050_15_20" --> "<testname>_<arg1>_<arg2>"

Commands:
---------
Step 1 - cd cfiles/
Step 2 - gcc -o parser.exe parser.c
Step 3 - parser.exe
(Keep the terminal open)

Open another terminal
Step1 - cd pyfiles/
Step2 - pytest -s -v