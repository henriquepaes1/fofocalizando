### Compilar e executar a fila e os consumidores

Dentro da pasta /new/messaging
```bash
g++ *.cpp -o queue 
./queue <n> 
```
Com n igual ao número de consumidores que se deseja criar

### Compilar e executar o produtor
Dentro da pasta /new/messaging_clients
```bash
g++ -o producer producer.cpp
./producer
```
