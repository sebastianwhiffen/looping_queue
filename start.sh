arg="$1"

if [ "$arg" == "m" ]; then
	gcc -g "./main.c" -o "main"
else

clang -O2 -g -fno-omit-frame-pointer \
  -DTRACY_ENABLE=1 -DTRACY_NO_EXIT=1 -DTRACY_ON_DEMAND=1 \
  -I./tracy/public -c main_profiled.c -o main.o

clang++ -O2 -g -fno-omit-frame-pointer  \
  -DTRACY_ENABLE=1 -DTRACY_NO_EXIT=1 -DTRACY_ON_DEMAND=1 \
  -I./tracy/public -Wno-deprecated-declarations \
  -c ./tracy/public/TracyClient.cpp -o TracyClient.o

fi

clang++ main.o TracyClient.o -pthread -o main

./main -t
