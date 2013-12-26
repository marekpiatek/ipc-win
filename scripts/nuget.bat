:: downloads dependencies used

set packager=..\tools\nuget\nuget.exe
set libs="..\lib"

%packager% install protobuf-net -OutputDirectory %libs%
%packager% install NSubstitute -OutputDirectory %libs%
%packager% install NUnit -OutputDirectory %libs%
%packager% install NStopwatch -OutputDirectory %libs%
%packager% install NDceRpc -OutputDirectory %libs%
%packager% install NRegFreeCom -OutputDirectory %libs%
%packager% install Google.ProtocolBuffers.Rpc -OutputDirectory %libs%






