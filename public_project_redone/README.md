# computacao-fisica-implementacao-de-projeto-publico-group-7

### Integrantes

- Karina Sousa Bahia
- Marcos Paulo de Oliveira

### Projeto original

https://github.com/TechTalkies/YouTube/tree/main/48%20ESP32%20Air%20Mouse

### Lista de componentes

| n   | Component                 |
| --- | ------------------------- |
| 1x  | ESP32                     |
| 1x  | MPU6050 Gyro (giroscópio) |
| 2x  | Botões                    |

## Instruções de uso

1. Conectar o ESP-WROOM-32 à energia

2. Ativar o bluetooth do dispositivo desejado (computador, celular, tablet...) e se conectar ao dispositivo "ESP32 Bluetooth Mouse".

O mouse atua como escravo para o dispositivo mestre que iniciará o contato. (compatibilidade confirmada com Android, Windows e Linux).

3. Caso não encontre de primeira, clique o botão 'EN' à esquerda da entrada de alimentação da placa e aguarde.

4. Uma vez conectado, ele deve funcionar imediatamente. Caso não, o giroscópio provavelmente está com mau contato, isso pode ser confirmado pelo monitor serial (115200) com as seguintes possibilidades:
   1. Failed to find MPU6050 chip **(mau contato)**
   2. MPU6050 Found! **(contato feito, aguardando ativação)**
   3. MPU6050 awakened! **(contato feito e ativado)**

## Entendimento código (de cima para baixo)

Para relizar a interpretação do código do projeto realizamremos um estudo como a busca em largura. Iniciaremos ententendo o "main", depois as funções chamadas (sendo biblioteca ou não) e repetimos essa última etapa até que tenhamos atingido uma das duas condições de parada:

1. Todo o código foi analizado
2. Encontrarmos código que seja mais baixo nível do que a proposta de análise desse ponto da disciplina.

### 47_EPS32_AirMOuse.ino

#### Bibliotecas utilizadas

```C++
#include <BleMouse.h>
#include <Adafruit_MPU6050.h>
```

#### Definições e variáveis globais

```C++
#define LEFTBUTTON 19
#define RIGHTBUTTON 18
#define SPEED 10

Adafruit_MPU6050 mpu;
BleMouse bleMouse;

bool sleepMPU = true;
long mpuDelayMillis;
```

#### Setup

```C++
void setup()
{
    Serial.begin(115200);

    pinMode(LEFTBUTTON, INPUT_PULLUP);
    pinMode(RIGHTBUTTON, INPUT_PULLUP);

    bleMouse.begin();

    delay(1000);
    // Try to initialize MPU!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    // Sleep MPU library till Bluetooth is connected as it seems to interrupt connection
    mpu.enableSleep(sleepMPU);
}
```

Fora as inicializações de pin e Serial básicas, aqui o código tenta inicial BleMouse e aguarda 1000ms para antes de tentar iniciar o giroscópio (MPU). Se o giroscípio não inicar ele fica em loop sem realiazar ação alguma (por isso é necessário reiniciar a placa). Depois a biblioteca do giroscópio fica em dormência para que seja reativada quando o bluetooth for conectado.

#### loop

```C++
void loop()
{
    if (bleMouse.isConnected())
    {
        if (sleepMPU)
        {
            delay(3000);
            Serial.println("MPU6050 awakened!");
            sleepMPU = false;
            mpu.enableSleep(sleepMPU);
            delay(500);
        }

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        // Depending on how you connected the MPU6050
        // You might need to swap these axeses or make them positive or negative
        bleMouse.move(g.gyro.z * -SPEED, g.gyro.x * -SPEED);

        if (!digitalRead(LEFTBUTTON))
        {
            Serial.println("Left click");
            bleMouse.click(MOUSE_LEFT);
            delay(500);
        }

        if (!digitalRead(RIGHTBUTTON))
        {
            Serial.println("Right click");
            bleMouse.click(MOUSE_RIGHT);
            delay(500);
        }
    }
}
```

Todo o loop está dentro do condicional: se BleMouse conectaodo. E realiza as seguintes etapas:

- Se MPU dormente, ativa MPU.
- Obtem movimentos do giroscópio `sensors_event_t a, g, temp;` e os envia para BleMouse para mover o cursor.
- Detecta se o botão direito e esquerdo foram precionados. O botão é configurado com o pullup imbutido do arduíno, por isso, o clique é identificado com nível lógico baixo, evidenciado pelo `!` em `if (!digitalRead(LEFTBUTTON))`

Chamadas para anlálise:

`2. Adafruit_MPU6050`, `mpu.begin`, `mpu.enableSleep`, `mpu.getEvent` e `sensor_event_t`.

`3. BleMouse`, `bleMouse.begin`, `BleMouse.isconnected`, `bleMouse.move` e `bleMouse.click`.

### AdaFruit_MPU6050

#### AdraFruit_MPU6050.h

```C++
class Adafruit_MPU6050 final {
    ...
    bool begin(uint8_t i2c_addr = MPU6050_I2CADDR_DEFAULT, TwoWire *wire = &Wire,
             int32_t sensorID = 0);
    ...
    bool enableSleep(bool enable);
    ...
    bool getEvent(sensors_event_t *accel, sensors_event_t *gyro,
                sensors_event_t *temp);
    ...
}
```
Essa classe é a classe final onde todos os outros objetos tipo Adafruit_MPU6050_XXXX são concentrados. Razão pela qual muitas das chamadas no main tem tipo Adafruit.begin, Adafruit.enableSleep e AdaFruit.getEvent.

#### Adafruit_MPU6050.cpp
##### begin
```C++
bool Adafruit_MPU6050::begin(uint8_t i2c_address, TwoWire *wire,
                             int32_t sensor_id) {
  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  // For boards with I2C bus power control, may need to delay to allow
  // MPU6050 to come up after initial power.
  bool mpu_found = false;
  for (uint8_t tries = 0; tries < 5; tries++) {
    mpu_found = i2c_dev->begin();
    if (mpu_found)
      break;
    delay(10);
  }
  if (!mpu_found)
    return false;

  Adafruit_BusIO_Register chip_id =
      Adafruit_BusIO_Register(i2c_dev, MPU6050_WHO_AM_I, 1);

  // make sure we're talking to the right chip
  if (chip_id.read() != MPU6050_DEVICE_ID) {
    return false;
  }

  return _init(sensor_id);
}
```
1. Erases any previous references.
2. Tries to connect a total of 5 times with 10ms delay between tries. (returns false if not found)
3. Registers the read chip and compares to the expected MPU6050_DEVICE_ID.
4. Returns the sensor id

##### enableSleep
```C++
bool Adafruit_MPU6050::enableSleep(bool enable) {
  Adafruit_BusIO_Register pwr_mgmt =
      Adafruit_BusIO_Register(i2c_dev, MPU6050_PWR_MGMT_1, 1);

  Adafruit_BusIO_RegisterBits sleep =
      Adafruit_BusIO_RegisterBits(&pwr_mgmt, 1, 6);
  return sleep.write(enable);
}
```
Registra enable para desativar entrada e saída de dados no BusIO.

##### getEvent
```C++
bool Adafruit_MPU6050::getEvent(sensors_event_t *accel, sensors_event_t *gyro,
                                sensors_event_t *temp) {
  uint32_t timestamp = millis();
  _read();

  fillTempEvent(temp, timestamp);
  fillAccelEvent(accel, timestamp);
  fillGyroEvent(gyro, timestamp);

  return true;
}
```
Escreve cada um dos valores para temperatura, aceleração e eixos do giroscópio nos ponteiros dados de parâmetro (*temp, *accel, *gyro). A temperatura é utilizada para calibração. Seguem as funções chamadas para escrita nas variáveis:

```C++
void Adafruit_MPU6050::fillTempEvent(sensors_event_t *temp,
                                     uint32_t timestamp) {

  memset(temp, 0, sizeof(sensors_event_t));
  temp->version = sizeof(sensors_event_t);
  temp->sensor_id = _sensorid_temp;
  temp->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
  temp->timestamp = timestamp;
  temp->temperature = temperature;
}

void Adafruit_MPU6050::fillAccelEvent(sensors_event_t *accel,
                                      uint32_t timestamp) {

  memset(accel, 0, sizeof(sensors_event_t));
  accel->version = 1;
  accel->sensor_id = _sensorid_accel;
  accel->type = SENSOR_TYPE_ACCELEROMETER;
  accel->timestamp = timestamp;
  accel->acceleration.x = accX * SENSORS_GRAVITY_STANDARD;
  accel->acceleration.y = accY * SENSORS_GRAVITY_STANDARD;
  accel->acceleration.z = accZ * SENSORS_GRAVITY_STANDARD;
}

void Adafruit_MPU6050::fillGyroEvent(sensors_event_t *gyro,
                                     uint32_t timestamp) {
  memset(gyro, 0, sizeof(sensors_event_t));
  gyro->version = 1;
  gyro->sensor_id = _sensorid_gyro;
  gyro->type = SENSOR_TYPE_GYROSCOPE;
  gyro->timestamp = timestamp;
  gyro->gyro.x = gyroX * SENSORS_DPS_TO_RADS;
  gyro->gyro.y = gyroY * SENSORS_DPS_TO_RADS;
  gyro->gyro.z = gyroZ * SENSORS_DPS_TO_RADS;
}
```


Chamadas para análise

`Wire.h`

`Adafruit_BusIO` `Adafruit_BusIO_Register`

### Adafruit_BusIO
#### Adafruit_BusIO_Register.h
##### Adafruit_BusIO_Register
A biblioteca BusIO é uma camada de abstração que esconde os detalhes dos protocolos de comunicação (I2C, SPI) atrás de classes mais simples. Os métodos de baixo nível (a transferência real de bytes pelo barramento) ficam em Adafruit_I2CDevice e Wire.h, que são tratados separadamente.

```C++
class Adafruit_BusIO_Register {
public:
  Adafruit_BusIO_Register(Adafruit_I2CDevice *i2cdevice, uint16_t reg_addr,
                          uint8_t width = 1, uint8_t byteorder = LSBFIRST,
                          uint8_t address_width = 1);
  ...
  bool read(uint8_t *buffer, uint8_t len);
  bool read(uint8_t *value);
  bool read(uint16_t *value);
  uint32_t read(void);
  uint32_t readCached(void);
  bool write(uint8_t *buffer, uint8_t len);
  bool write(uint32_t value, uint8_t numbytes = 0);
  ...
private:
  Adafruit_I2CDevice *_i2cdevice;
  Adafruit_SPIDevice *_spidevice;
  Adafruit_GenericDevice *_genericdevice;
  Adafruit_BusIO_SPIRegType _spiregtype;
  uint16_t _address;
  uint8_t _width, _addrwidth, _byteorder;
  uint8_t _buffer[4];
  uint32_t _cached = 0;
};
```
Essa classe representa um registrador de um dispositivo externo (no nosso caso, um registrador interno do MPU6050). Em vez de o programador montar manualmente cada transação I2C, ele cria um objeto que “aponta” para o registrador e usa read e write.

##### Adafruit_BusIO_RegisterBits
```C++
class Adafruit_BusIO_RegisterBits {
public:
  Adafruit_BusIO_RegisterBits(Adafruit_BusIO_Register *reg, uint8_t bits,
                              uint8_t shift);
  bool write(uint32_t value);
  uint32_t read(void);

private:
  Adafruit_BusIO_Register *_register;
  uint8_t _bits, _shift;
};
```

Essa classe representa uma fatia de bits dentro de um registrador. Muitos registradores guardam várias configurações independentes, cada uma ocupando alguns bits. Em vez de manipular máscaras manualmente, o programador define a fatia e lê ou escreve só ela.

Chamadas para análise

`Wire.h`

`Adafruit_I2CDevice` 

### Adafruit_I2Device
```C++
class Adafruit_I2CDevice {
public:
  Adafruit_I2CDevice(uint8_t addr, TwoWire *theWire = &Wire);
  uint8_t address(void);
  bool begin(bool addr_detect = true);
  void end(void);
  bool detected(void);

  bool read(uint8_t *buffer, size_t len, bool stop = true);
  bool write(const uint8_t *buffer, size_t len, bool stop = true,
             const uint8_t *prefix_buffer = nullptr, size_t prefix_len = 0);
  bool write_then_read(const uint8_t *write_buffer, size_t write_len,
                       uint8_t *read_buffer, size_t read_len,
                       bool stop = false);
  bool setSpeed(uint32_t desiredclk);

  /*!   @brief  How many bytes we can read in a transaction
   *    @return The size of the Wire receive/transmit buffer */
  size_t maxBufferSize() { return _maxBufferSize; }

private:
  uint8_t _addr;
  TwoWire *_wire;
  bool _begun;
  size_t _maxBufferSize;
  bool _read(uint8_t *buffer, size_t len, bool stop);
};
```
A classe Adafruit_I2CDevice representa um dispositivo escravo no barramento I2C, identificado pelo seu endereço de 7 bits (MPU6050). Ela é o que está por baixo do Adafruit_BusIO_Register: enquanto o Register controla qual registrador ler ou escrever, o I2CDevice controla como conversar com o dispositivo no barramento. Por sua vez, ela delega a transferência real de bytes para a classe TwoWire.

Chamadas para análise

`Wire.h` `TwoWire` `Write.write` `Wire.endTransmission` `Wire.requestFrom` `Wire.read`

### Wire
#### Wire.h (TwoWire)
```C++
class TwoWire : public HardwareI2C {
protected:
  uint8_t num;
  int8_t sda;
  int8_t scl;
  uint8_t *rxBuffer;
  uint8_t *txBuffer;
  uint16_t txAddress;
  ...
public:
  TwoWire(uint8_t bus_num);
  bool begin() override final { return begin(-1, -1); }
  bool end() override;
  bool setClock(uint32_t freq) override;

  void beginTransmission(uint8_t address) override;
  uint8_t endTransmission(bool stopBit) override;
  uint8_t endTransmission() override;

  size_t requestFrom(uint8_t address, size_t len, bool stopBit) override;
  size_t requestFrom(uint8_t address, size_t len) override;

  bool setPins(int sda, int scl);
  bool begin(int sda, int scl, uint32_t frequency = 0);
  size_t setBufferSize(size_t bSize);
  void setTimeOut(uint16_t timeOutMillis);

  size_t write(uint8_t) override;
  size_t write(const uint8_t *, size_t) override;
  int available() override;
  int read() override;
  ...
};

extern TwoWire Wire;
```
A classe TwoWire representa um controlador I2C do ESP32 e implementa a interface HardwareI2C. O objeto global `Wire` (declarado com `extern` no final do arquivo) usa o primeiro controlador e é o que o Adafruit_I2CDevice recebe como padrão. O `begin()` inicia o barramento como mestre nos pinos padrão (SDA 21 e SCL 22) e `setClock` define a frequência. A comunicação segue o padrão: `beginTransmission` guarda o endereço do dispositivo, `write` acumula bytes no buffer de envio (128 bytes por padrão) e `endTransmission` envia o buffer, retornando 0 em caso de sucesso. Para leitura, `requestFrom` solicita `len` bytes ao dispositivo e os guarda no buffer de recepção, e `available` e `read` consomem esses bytes um a um. É aqui que o código deixa de ser lógica de biblioteca e passa a usar o hardware, por isso é o ponto de parada da análise.

Chamadas para análise

`esp32-hal-i2c.c` e driver I2C do ESP-IDF

### BleMouse

#### BleMouse.h

```C++
BleMouse::BleMouse(std::string deviceName, std::string deviceManufacturer, uint8_t batteryLevel) {
    this->deviceName = deviceName;
    this->deviceManufacturer = deviceManufacturer;
    this->batteryLevel = batteryLevel;
    this->connectionStatus = new BleConnectionStatus();
    this->_buttons = 0;
}
```
O construtor configura as cadeias de caracteres que representam o nome visível do dispositivo durante a varredura Bluetooth (deviceName) e a identificação do fabricante (deviceManufacturer), além de atribuir o nível percentual de bateria (batteryLevel). Ele define a variável privada _buttons como 0, garantindo que nenhum botão esteja assinalado como pressionado no momento da energização. Além disso, aloca dinamicamente um objeto da classe BleConnectionStatus, herdada da interface BLEServerCallbacks, que atuará como o manipulador de eventos de conexão do servidor BLE

Chamadas para análise

`deviceName` `deviceManufacturer` `batteryLevel`

`BLEServerCallbacks`

`BleConnectionStatus`

#### BleMouse::begin

```C++
void BleMouse::begin(void) {
    xTaskCreate(this->taskServer, "server", 20000, (void *)this, 5, NULL);
}
```
A chamada do método begin contorna o bloqueio da thread principal de execução (loopTask do Arduino) invocando a rotina xTaskCreate da API do FreeRTOS. A função aloca uma nova tarefa nomeada "server" com uma pilha de memória de 20.000 bytes, necessária para acomodar as alocações da pilha de protocolos Bluetooth do ESP32 e o registro das tabelas GATT. O ponteiro this é passado como parâmetro de contexto (void *), permitindo que a função estática taskServer acesse as variáveis e ponteiros da instância ativa de BleMouse.

Chamadas para análise

`taskServer`

#### BleMouse::isConnected

```C++
bool BleMouse::isConnected(void) {
    return this->connectionStatus->connected;
}
```
A verificação de conexão realiza uma leitura direta do membro booleano connected pertencente à instância de BleConnectionStatus. Por se tratar de um acesso a uma variável mantida em memória e atualizada assincronamente por callbacks da pilha Bluetooth, a função executa em tempo constante $O(1)$ sem introduzir latência de comunicação por rádio, permitindo sua chamada contínua dentro da estrutura if no loop do sketch.

Chamadas para análise

`BleConnectionStatus`


#### BleMouse::move

```C++
void BleMouse::move(signed char x, signed char y, signed char wheel, signed char hWheel) {
    if (this->isConnected()) {
        uint8_t m[5];
        m[0] = this->_buttons;
        m[1] = x;
        m[2] = y;
        m[3] = wheel;
        m[4] = hWheel;
        this->rawAction(m, sizeof(m));
    }
}
```
O método avalia previamente se o dispositivo está conectado. Caso afirmativo, aloca um vetor local de 5 bytes não assinados (uint8_t m[5]): o primeiro byte (m[0]) armazena a máscara de bits dos botões ativos (_buttons); os bytes subseqüentes (m[1] a m[4]) recebem inteiros assinados de 8 bits (signed char), variando no intervalo de -128 a +127, que representam respectivamente os deltas de movimento no eixo $X$, eixo $Y$, roda de rolagem vertical (wheel) e rolagem horizontal (hWheel). A estrutura montada é imediatamente repassada à função privada rawAction.

Chamadas para análise

`rawAction`

#### BleMouse::click

```C++
void BleMouse::click(uint8_t b) {
    this->press(b);
    this->release(b);
}
```
A função abstrai um clique completo encadeando as chamadas press(b) e release(b). Essa abordagem faz com que o dispositivo envie dois relatórios HID consecutivos ao sistema hospedeiro: o primeiro relatando a transição do botão para o estado pressionado e o segundo redefinindo a máscara de bits, sinalizando a liberação do botão sem a necessidade de intervenção do programador para controlar o tempo entre estados no nível da aplicação.

Chamadas para análise

`press` `release`

#### setBatteryLevel

```C++
void BleMouse::setBatteryLevel(uint8_t level) {
    this->batteryLevel = level;
    if (this->hid != nullptr) {
        this->hid->setBatteryLevel(level);
    }
}
```

#### end
```C++
void BleMouse::end(void) {
}
```

O método setBatteryLevel atualiza o valor da propriedade local e, se a infraestrutura HID já tiver sido inicializada (hid != nullptr), envia a informação de carga para o serviço de Bateria GATT (Battery Service). O método end é mantido vazio por limitação da estrutura de desconexão estática do driver nativo no ecossistema Arduino-ESP32, servindo como uma interface de compatibilidade de código.

#### BleConnectionStatus

```C++
BleConnectionStatus::BleConnectionStatus(void) {
    this->connected = false;
}

void BleConnectionStatus::onConnect(BLEServer* pServer) {
    this->connected = true;
}

void BleConnectionStatus::onDisconnect(BLEServer* pServer) {
    this->connected = false;
    pServer->getAdvertising()->start();
}
```
Ao herdar de BLEServerCallbacks, a classe reescreve os métodos onConnect e onDisconnect. Quando a conexão estabelecida é confirmada pelo stack, onConnect altera a variável booleana connected para true. Na ocorrência de desconexão, onDisconnect atualiza o estado para false e imediatamente reinicia o processo de anúncios de rádio chamando pServer->getAdvertising()->start(), permitindo que o ESP32 volte a ser visível para eventuais tentativas de reconexão do hospedeiro.

Chamadas para análise

`BLEServerCallbacks` `pServer->getAdvertising()->start()`




#### BleMouse::taskServer

```C++
static const uint8_t _hidReportDescriptor[] = {
  USAGE_PAGE(1),       0x01, // Generic Desktop
  USAGE(1),            0x02, // Mouse
  COLLECTION(1),       0x01, // Application
  USAGE(1),            0x01, // Pointer
  COLLECTION(1),       0x00, // Physical
  USAGE_PAGE(1),       0x09, // Button
  USAGE_MINIMUM(1),    0x01, // Button 1
  USAGE_MAXIMUM(1),    0x05, // Button 5
  LOGICAL_MINIMUM(1),  0x00,
  LOGICAL_MAXIMUM(1),  0x01,
  REPORT_COUNT(1),     0x05, // 5 bits para 5 botões
  REPORT_SIZE(1),      0x01,
  INPUT(1),            0x02, // Data, Variable, Absolute
  REPORT_COUNT(1),     0x01, // 3 bits de preenchimento (padding)
  REPORT_SIZE(1),      0x03,
  INPUT(1),            0x01, // Constant (Padding)
  USAGE_PAGE(1),       0x01, // Generic Desktop
  USAGE(1),            0x030, // X
  USAGE(1),            0x031, // Y
  USAGE(1),            0x038, // Wheel
  LOGICAL_MINIMUM(1),  0x81, // -127
  LOGICAL_MAXIMUM(1),  0x7f, // 127
  REPORT_SIZE(1),      0x08, // 8 bits por eixo
  REPORT_COUNT(1),     0x03, // X, Y, Wheel
  INPUT(1),            0x06, // Data, Variable, Relative
  USAGE_PAGE(1),       0x0c, // Consumer Devices
  USAGE(2),            0x38, 0x02, // AC Pan (Rolagem Horizontal)
  LOGICAL_MINIMUM(1),  0x81, // -127
  LOGICAL_MAXIMUM(1),  0x7f, // 127
  REPORT_SIZE(1),      0x08,
  REPORT_COUNT(1),     0x01,
  INPUT(1),            0x06, // Data, Variable, Relative
  END_COLLECTION(0),
  END_COLLECTION(0)
};

void BleMouse::taskServer(void* pvParameter) {
    BleMouse* bleMouseInstance = (BleMouse *) pvParameter;
    BLEDevice::init(bleMouseInstance->deviceName);
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(bleMouseInstance->connectionStatus);

    bleMouseInstance->hid = new BLEHIDDevice(pServer);
    bleMouseInstance->inputMouse = bleMouseInstance->hid->inputReport(1);

    bleMouseInstance->hid->manufacturer()->setValue(bleMouseInstance->deviceManufacturer);
    bleMouseInstance->hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    bleMouseInstance->hid->hidInfo(0x00, 0x01);

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    bleMouseInstance->hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
    bleMouseInstance->hid->startServices();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(bleMouseInstance->hid->hidService()->getUUID());
    pAdvertising->start();

    bleMouseInstance->hid->setBatteryLevel(bleMouseInstance->batteryLevel);
    bleMouseInstance->onStarted(pServer);

    vTaskDelete(NULL);
}
```
A função recupera a referência da instância através da conversão do ponteiro pvParameter. A constante _hidReportDescriptor estrutura o protocolo interpretado pelo sistema operacional: define 5 bits para os botões do mouse, acrescidos de 3 bits de preenchimento alinhados em 1 byte; define em seguida 3 bytes assinados referentes aos eixos $X$, $Y$ e roda de rolagem vertical, finalizando com 1 byte para a rolagem horizontal.A tarefa inicializa o suporte a BLE via BLEDevice::init, aloca o servidor e associa a classe de callbacks BleConnectionStatus. Instancia o objeto BLEHIDDevice, aloca o relatório de entrada, e injeta os parâmetros de autenticação e criptografia exigidos por sistemas como Windows e iOS (ESP_LE_AUTH_BOND). Após registrar o mapa de relatórios, os serviços HID são iniciados e os dados de sinalização pública (advertising) são configurados com a aparência HID_MOUSE. Concluída a inicialização, a função executa vTaskDelete(NULL), encerrando a tarefa no FreeRTOS e liberando a memória alocada para sua execução.

Chamadas para análise

`BLEHIDDevice`

#### BleMouse::rawAction

```C++
void BleMouse::rawAction(uint8_t msg[], char msgSize) {
    this->inputMouse->setValue(msg, msgSize);
    this->inputMouse->notify();
}
```
A função rawAction atua como a interface direta com o stack de Bluetooth nativo do ESP32. O método setValue copia o vetor de 5 bytes para a memória da característica GATT configurada para relatórios de entrada HID. Na sequência, a chamada do método notify instrui a pilha do protocolo a enviar um pacote de notificação sem exigência de confirmação para o dispositivo hospedeiro. Esse padrão reduz a sobrecarga de canal e minimiza a latência na atualização do cursor do sistema operacional.

#### BleMouse::press 
```C++
void BleMouse::press(uint8_t b) {
    this->buttons(this->_buttons | b);
}
````

#### BleMouse::release
```C++
void BleMouse::release(uint8_t b) {
    this->buttons(this->_buttons & ~b);
}
````

#### BleMouse::buttons
```C++
void BleMouse::buttons(uint8_t b) {
    if (b != this->_buttons) {
        this->_buttons = b;
        move(0, 0, 0, 0);
    }
}void BleMouse::buttons(uint8_t b) {
    if (b != this->_buttons) {
        this->_buttons = b;
        move(0, 0, 0, 0);
    }
}
````

Os métodos press e release utilizam álgebra booleana para alterar o mapa de bits interno. O método press aplica o operador binário OU (|), definindo como ativo o bit correspondente à constante enviada (por exemplo, MOUSE_LEFT = 1, MOUSE_RIGHT = 2), mantendo o estado dos demais botões. O método release aplica uma combinação de negação binária (~) com o operador E (&), zerando exclusivamente o bit alvo. Ambas as funções direcionam o resultado para buttons(uint8_t b), que compara a nova máscara com a armazenada. Havendo divergência, atualiza _buttons e invoca move(0, 0, 0, 0), transmitindo um relatório contendo a nova configuração de botões sem deslocamento posicional.