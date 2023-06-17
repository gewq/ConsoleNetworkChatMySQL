BIN = client

CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra -pedantic

#Каталог с *.o файлами
objects_dir := obj

#Каталоги с исходными текстами
source_dirs := .
source_dirs += Initialization/
source_dirs += Initialization/Exceptions
source_dirs += Server/
source_dirs += Server/Exceptions
source_dirs += State/
source_dirs += User/
source_dirs += Message/
source_dirs += SHA_1/
source_dirs += TinyXML/

source_dirs += Settings/
source_dirs += Settings/Exceptions

source_dirs += Chat/
source_dirs += Chat/States/AddresseeInput
source_dirs += Chat/States/AddresseeMissing
source_dirs += Chat/States/CreateLogin
source_dirs += Chat/States/CreateNickname
source_dirs += Chat/States/CreatePassword
source_dirs += Chat/States/LoginInput
source_dirs += Chat/States/LoginRegistered
source_dirs += Chat/States/LoginUnregistered
source_dirs += Chat/States/PasswordIncorrect
source_dirs += Chat/States/PasswordInput
source_dirs += Chat/States/Start
source_dirs += Chat/States/UserInChat


search_wildcards := $(addsuffix /*.cpp,$(source_dirs))

#Список объектных файлов для сборки программы
objectsFile := $(notdir $(patsubst %.cpp,obj/%.o,$(wildcard $(search_wildcards))))

#Список объектных файлов вместе с директорией в которую их помещать
objectsPath := $(addprefix $(objects_dir)/,$(objectsFile))


all: $(BIN)

VPATH := $(source_dirs)

$(BIN): $(objectsPath)
	$(CXX) $^ $(CXXFLAGS) -o $@

$(objects_dir)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -MD $(addprefix -I,$(source_dirs)) $< -o $@

include $(wildcard *.d)

clean:
	rm *.o *.d