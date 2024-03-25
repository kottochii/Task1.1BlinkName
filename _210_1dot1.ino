#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#define BUTTON 
#define TIME_UNIT 1000

#define BUTTON_PIN  3

class Signal
{
  public:
  virtual void run() = 0;
  virtual void print()
  {
    Serial.println("Signal");
  }
};

typedef std::vector<std::shared_ptr<Signal>> signal_collection_t;
bool keep_going = true;

class NullSignal : public Signal
{
  public:
  void run() {}
};

class Dot : public Signal
{
  public:
  void run()
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(TIME_UNIT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(TIME_UNIT);
  }
  void print()
  {
    Serial.println("Dot");
  }
};

class Dash : public Signal
{
  public:
  void run()
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(3 * TIME_UNIT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(TIME_UNIT);
  }
  void print()
  {
    Serial.println("Dash");
  }
};
class ShortGap : public Signal
{
  public:
  void run()
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(TIME_UNIT);
  }
  void print()
  {
    Serial.println("ShortGap");
  }
};
class MedGap : public Signal
{
  public:
  void run()
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(7 * TIME_UNIT);
  }
  void print()
  {
    Serial.println("MedGap");
  }
};

std::unordered_map<char, signal_collection_t> _char_signal_map;

/**
  Makes one from Signal Collection from string literal where:
  . denotes dot (one TIME_UNIT long HIGH, then one TIME_UNIT long LOW)
  - denotes dash (three TIME_UNIT long HIGH, then one TIME_UNIT long LOW)
  [SPACE] denotes short gap (three TIME_UNIT long LOW)
*/
signal_collection_t operator""signalcollection(const char* signal_string, unsigned length)
{
  signal_collection_t _returnable;
  for(int i = 0; i < length; i++)
  {
    if(signal_string[i] == '-')
    {
      _returnable.push_back(std::make_shared<Dash>());
    }
    else if(signal_string[i] == '.')
    {
      _returnable.push_back(std::make_shared<Dot>());
    }
    else if(signal_string[i] == ' ')
    {
      _returnable.push_back(std::make_shared<MedGap>());
    }
  }
  return _returnable;
}

void setupAlphabet()
{
  _char_signal_map.insert(std::pair<char, signal_collection_t>('A', ".-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('B', "-..."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('C', "-.-."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('D', "-.."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('E', "."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('F', "..-."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('G', "--."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('H', "...."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('I', ".."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('J', ".---"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('K', "-.-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('L', ".-.."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('M', "--"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('N', "-."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('O', "---"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('P', ".--."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('Q', "--.-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('R', ".-."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('S', "..."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('T', "-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('U', "..-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('V', "...-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('W', ".--"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('X', "-..-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('Y', "-.--"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('Z', "--.."signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>(' ', " "signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('-', "-"signalcollection ));
  _char_signal_map.insert(std::pair<char, signal_collection_t>('.', "."signalcollection ));
}


void blinkMorseMessage(const std::string& message)
{
  ShortGap shortgap;
  for(int i = 0; i < message.length(); i++)
  {
    if(!keep_going)
      return;
    Serial.println(message[i]);
    if(i != 0)
    {
      shortgap.run();
      shortgap.print();
    }
    std::unordered_map<char, signal_collection_t>::iterator colxn_iterator = _char_signal_map.find(message[i]);
    if(colxn_iterator != _char_signal_map.end())
    {
      blinkSignalCollection(colxn_iterator->second);
    }
  }
  if(!keep_going)
    return;
  shortgap.run();
  shortgap.print();
}

void blinkSignalCollection(const signal_collection_t& collection)
{
  for(int i = 0; i < collection.size(); i++)
  {
    if(!keep_going)
      return;
    collection[i]->print();
    collection[i]->run();
  }
}



void setup() {
  setupAlphabet();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, LOW);
}

void onButtonPressed()
{
  Serial.println("onButtonPressed");
  keep_going = !keep_going;
}


void loop() {
  if(keep_going)
    blinkMorseMessage("KO");
//  Serial.println(digitalRead(BUTTON_PIN));
//  delay(100);
}
