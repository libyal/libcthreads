# Tests library functions and types.

$LibraryTests = "condition error lock mutex queue read_write_lock repeating_thread support thread thread_attributes thread_pool"
$LibraryTestsWithInput = ""
$OptionSets = "" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTestBinary ${TestExecutablesDirectory} "cthreads_test_${TestName}"

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

$TestInputs = GenerateTestInputs "libcthreads" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$Result = RunTestBinaryWithInput ${TestExecutablesDirectory} "cthreads_test_${TestName}" ${TestInput}

		If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
		{
			Break
		}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}
