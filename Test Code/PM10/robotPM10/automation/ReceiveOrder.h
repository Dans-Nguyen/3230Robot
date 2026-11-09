/*
  Receives a repeating order sent by a modulated IR kiosk.
  Call update() continuously from loop().  After an end code and its following
  off interval are seen, available() becomes true.
*/
#ifndef RECEIVE_ORDER_H
#define RECEIVE_ORDER_H

class ReceiveOrder {
public:
  enum BlockColor : uint8_t { NONE, RED, YELLOW, BLUE, BROWN };

  static const uint8_t MAX_BLOCKS = 10;

  // 8000 us is longer than a 300 Hz period (3333 us), but well below the
  // 100 ms dark interval between symbols.
  ReceiveOrder(LTR_4206E_IR& ir, float toleranceHz = 55.0f)
      : _ir(ir), _tolerance(toleranceHz), _count(0), _sampleCode(NONE),
        _sameSamples(0), _pendingCode(NONE), _waitingForGap(false),
        _receiving(false), _complete(false) {}

  // Call as often as possible. Returns true once when a complete new order
  // has been received.
  bool update() {
    const BlockColor reading = decode(_ir.getFrequency());

    if (reading == NONE) {
      // A confirmed symbol is committed only after its dark separator.
      if (_waitingForGap) {
        const bool wasComplete = _complete;
        handleSymbol(_pendingCode);
        _waitingForGap = false;
        _pendingCode = NONE;
        if (!wasComplete && _complete) return true;
      }
      _sampleCode = NONE;
      _sameSamples = 0;
      return false;
    }

    if (reading == INVALID) {
      // Do not mistake an unrecognized/noisy frequency for the dark gap.
      _sampleCode = NONE;
      _sameSamples = 0;
      return false;
    }

    // Reject a one-sample transition glitch: require two consecutive samples
    // that decode to the same frequency code.
    if (reading == _sampleCode) {
      if (_sameSamples < 255) _sameSamples++;
    } else {
      _sampleCode = reading;
      _sameSamples = 1;
    }

    if (_sameSamples >= 2 && !_waitingForGap) {
      _pendingCode = reading;
      _waitingForGap = true;
    }
    return false;
  }

  bool available() const { return _complete; }
  uint8_t count() const { return _count; }
  BlockColor block(uint8_t index) const {
    return (index < _count) ? _blocks[index] : NONE;
  }

  // Call after your robot has copied/acted on the current order.
  void clear() {
    _count = 0;
    _receiving = false;
    _complete = false;
    _waitingForGap = false;
    _pendingCode = NONE;
  }

private:
  // Internal marker codes; they cannot be returned by block().
  static const BlockColor START = static_cast<BlockColor>(5);
  static const BlockColor END   = static_cast<BlockColor>(6);
  static const BlockColor INVALID = static_cast<BlockColor>(7);

  LTR_4206E_IR& _ir;
  float _tolerance;
  BlockColor _blocks[MAX_BLOCKS];
  uint8_t _count;
  BlockColor _sampleCode;
  uint8_t _sameSamples;
  BlockColor _pendingCode;
  bool _waitingForGap;
  bool _receiving;
  bool _complete;

  BlockColor decode(float frequency) const {
    if (frequency <= 0.0f) return NONE;
    if (near(frequency, 300)) return START;
    if (near(frequency, 400)) return RED;
    if (near(frequency, 500)) return YELLOW;
    if (near(frequency, 600)) return BLUE;
    if (near(frequency, 700)) return BROWN;
    if (near(frequency, 800)) return END;
    return INVALID;
  }

  bool near(float measured, float expected) const {
    return abs(measured - expected) <= _tolerance;
  }

  void handleSymbol(BlockColor code) {
    if (code == START) {
      _count = 0;
      _receiving = true;
      _complete = false;
      return;
    }

    if (!_receiving) return;

    if (code == END) {
      _receiving = false;
      _complete = true;
      return;
    }

    if (_count < MAX_BLOCKS) _blocks[_count++] = code;
  }
};

#endif


