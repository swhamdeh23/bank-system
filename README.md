# Bank Client Management System (C++)

A console application that manages bank clients, stored in a plain text file.
Built as a university practice project to learn file handling, structs, vectors, enums and menu-driven programs in C++.

## Features

- Show all clients
- Add a new client
- Delete a client
- Update client info
- Find a client by account number
- Transactions: deposit, withdraw, show total balances

## How it works

```
Client.txt  --Load-->  vector of clients  --edit-->  Save  -->  Client.txt
```

Each client is one line in `Client.txt`, fields separated by `#//#`:

```
A101#//#1234#//#Ali Ahmad#//#0790000000#//#500.000000
```

(Use fake data only when testing.)

## Project structure

| File | Purpose |
|---|---|
| `Bank.h` | All the program logic (structs, file handling, screens, menus) |
| `main.cpp` | Entry point, calls `MainMenuScreen()` |

## Build and run

The program uses `system("cls")` and `system("pause")`, so it runs on **Windows**.

```bash
g++ main.cpp -o bank
./bank
```

Or open the folder in Visual Studio and run `main.cpp`.

## Known issues / planned fixes

- `SaveClientsDataToFile` also asks for the new values when updating; it should only write to the file.
- Withdraw uses `<` instead of `<=`, so the full balance cannot be withdrawn.
- The balances screen counts clients before reloading the file.
- The file name is hard-coded in a few places instead of using `ClientsFileName`.
- Phone and name inputs using `cin >>` do not accept spaces.
- No duplicate account number check when adding a client.
