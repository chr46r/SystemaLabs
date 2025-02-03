## Directory Structure

```
mvp-blockchain/
├── cobol-ledger/       # COBOL-based ledger
├── cpp-consensus/      # C++ PoA node
├── docker/             # Docker config (optional)
├── pos-simulator/      # POS simulator in C++
├── android-wallet/     # Android Studio project
└── README.md
```

## How to Build and Run

1. **COBOL Ledger**  
   ```bash
   cd cobol-ledger
   ./build.sh  # or cobc -x -o ledger ledger.cbl
   ```
   This produces an executable `ledger`.

2. **C++ PoA Node**  
   - With **CMake**:
     ```bash
     cd cpp-consensus
     mkdir build && cd build
     cmake ..
     make
     ```
     This produces `poa_node`.  
   - Or directly with **g++**:
     ```bash
     g++ main.cpp ledger_integration.cpp pq_crypto.cpp -pthread -o poa_node
     ```

3. **Run Nodes**  
   In two separate terminals:
   ```bash
   # Terminal 1
   ./poa_node 1 5001 127.0.0.1 5002

   # Terminal 2
   ./poa_node 2 5002 127.0.0.1 5001
   ```

4. **Regulator Dashboard**  
   ```bash
   cd cpp-consensus
   python3 app.py
   ```
   Open [http://127.0.0.1:8080](http://127.0.0.1:8080).  
   Make sure you run `app.py` from the directory containing `blocks.log` or use absolute paths.

5. **POS Simulator (Optional)**  
   ```bash
   cd pos-simulator
   g++ pos_main.cpp -o pos_sim
   ./pos_sim
   ```
   Enter your `sender`, `receiver`, and `amount`. This sends a TX to Node1 on port 5001.

6. **Android Wallet**  
   - Import `android-wallet/` into Android Studio.
   - Edit IP/Port in `MainActivity.kt` to match your node.
   - Run the app in an emulator.  
   - Type in `sender, receiver, amount` → “Send Transaction” → see results in console and `blocks.log`.

## Notes

- Ensure the node writes `blocks.log` in the same path that `app.py` expects.
- The COBOL ledger is just an example; adapt for real usage.

---

