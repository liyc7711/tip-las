CXXFLAGS = -O2  -std=c++11 -I./

OBJS =  ./utility/SentenceSplit.o ./utility/StringOperation.o ./utility/POSExtract.o \
		./utility/StringSplit.o ./utility/StringType.o ./utility/TextClassification.o \
        ./utility/Tokenize.o  ./tagging/Sample.o  ./tagging/Model.o  ./tagging/Decoder.o \
        ./tagging/Perceptron.o ./Formatting.o  ./Las.o ./Main.o
# tip platform
tip-las: $(OBJS)
	g++ -g -o ./$@ $^ -lpthread -ldl 

tip-las.i386: $(OBJS)
	g++ -D__MACHINE_TYPE_32__ -o ./tip-tws $^ -lpthread -ldl -m32 

$(OBJS): %.o:%.cpp
	g++ $(CXXFLAGS) -o $@ -c $< 

.PHONY: clean

clean:
	-rm $(OBJS) ./tip-las
