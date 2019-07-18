##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Server
ConfigurationName      :=Debug
WorkspacePath          :=D:/DeveloperFolder/Magic
ProjectPath            :=D:/DeveloperFolder/Magic/Server
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=61419
Date                   :=2019/7/18
CodeLitePath           :="D:/Program Files/CodeLite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="Server.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)D:\DeveloperTool\GCC\mingw64\x86_64-w64-mingw32\include $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS := -std=c++11 -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=D:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/Src_Log_logAppender.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Log_logEvent.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Log_logFormatter.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_main.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Log_logWrap.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Log_logger.cc$(ObjectSuffix) $(IntermediateDirectory)/Src_Log_logLevel.cc$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Src_Log_logAppender.cc$(ObjectSuffix): Src/Log/logAppender.cc $(IntermediateDirectory)/Src_Log_logAppender.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logAppender.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logAppender.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logAppender.cc$(DependSuffix): Src/Log/logAppender.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logAppender.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logAppender.cc$(DependSuffix) -MM Src/Log/logAppender.cc

$(IntermediateDirectory)/Src_Log_logAppender.cc$(PreprocessSuffix): Src/Log/logAppender.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logAppender.cc$(PreprocessSuffix) Src/Log/logAppender.cc

$(IntermediateDirectory)/Src_Log_logEvent.cc$(ObjectSuffix): Src/Log/logEvent.cc $(IntermediateDirectory)/Src_Log_logEvent.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logEvent.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logEvent.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logEvent.cc$(DependSuffix): Src/Log/logEvent.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logEvent.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logEvent.cc$(DependSuffix) -MM Src/Log/logEvent.cc

$(IntermediateDirectory)/Src_Log_logEvent.cc$(PreprocessSuffix): Src/Log/logEvent.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logEvent.cc$(PreprocessSuffix) Src/Log/logEvent.cc

$(IntermediateDirectory)/Src_Log_logFormatter.cc$(ObjectSuffix): Src/Log/logFormatter.cc $(IntermediateDirectory)/Src_Log_logFormatter.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logFormatter.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logFormatter.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logFormatter.cc$(DependSuffix): Src/Log/logFormatter.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logFormatter.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logFormatter.cc$(DependSuffix) -MM Src/Log/logFormatter.cc

$(IntermediateDirectory)/Src_Log_logFormatter.cc$(PreprocessSuffix): Src/Log/logFormatter.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logFormatter.cc$(PreprocessSuffix) Src/Log/logFormatter.cc

$(IntermediateDirectory)/Src_main.cc$(ObjectSuffix): Src/main.cc $(IntermediateDirectory)/Src_main.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/main.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_main.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_main.cc$(DependSuffix): Src/main.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_main.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_main.cc$(DependSuffix) -MM Src/main.cc

$(IntermediateDirectory)/Src_main.cc$(PreprocessSuffix): Src/main.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_main.cc$(PreprocessSuffix) Src/main.cc

$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(ObjectSuffix): Src/Util/Adapter/api_Window.cc $(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Util/Adapter/api_Window.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(DependSuffix): Src/Util/Adapter/api_Window.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(DependSuffix) -MM Src/Util/Adapter/api_Window.cc

$(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(PreprocessSuffix): Src/Util/Adapter/api_Window.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Util_Adapter_api_Window.cc$(PreprocessSuffix) Src/Util/Adapter/api_Window.cc

$(IntermediateDirectory)/Src_Log_logWrap.cc$(ObjectSuffix): Src/Log/logWrap.cc $(IntermediateDirectory)/Src_Log_logWrap.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logWrap.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logWrap.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logWrap.cc$(DependSuffix): Src/Log/logWrap.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logWrap.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logWrap.cc$(DependSuffix) -MM Src/Log/logWrap.cc

$(IntermediateDirectory)/Src_Log_logWrap.cc$(PreprocessSuffix): Src/Log/logWrap.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logWrap.cc$(PreprocessSuffix) Src/Log/logWrap.cc

$(IntermediateDirectory)/Src_Log_logger.cc$(ObjectSuffix): Src/Log/logger.cc $(IntermediateDirectory)/Src_Log_logger.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logger.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logger.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logger.cc$(DependSuffix): Src/Log/logger.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logger.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logger.cc$(DependSuffix) -MM Src/Log/logger.cc

$(IntermediateDirectory)/Src_Log_logger.cc$(PreprocessSuffix): Src/Log/logger.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logger.cc$(PreprocessSuffix) Src/Log/logger.cc

$(IntermediateDirectory)/Src_Log_logLevel.cc$(ObjectSuffix): Src/Log/logLevel.cc $(IntermediateDirectory)/Src_Log_logLevel.cc$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/DeveloperFolder/Magic/Server/Src/Log/logLevel.cc" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Src_Log_logLevel.cc$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Src_Log_logLevel.cc$(DependSuffix): Src/Log/logLevel.cc
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Src_Log_logLevel.cc$(ObjectSuffix) -MF$(IntermediateDirectory)/Src_Log_logLevel.cc$(DependSuffix) -MM Src/Log/logLevel.cc

$(IntermediateDirectory)/Src_Log_logLevel.cc$(PreprocessSuffix): Src/Log/logLevel.cc
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Src_Log_logLevel.cc$(PreprocessSuffix) Src/Log/logLevel.cc


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


