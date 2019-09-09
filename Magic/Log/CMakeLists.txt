add_definitions(
	#Not Security Check
	-DNSC
	)
aux_source_directory(. DIR_MODULE)
add_library(Log ${DIR_MODULE})
