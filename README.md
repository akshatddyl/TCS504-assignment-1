# Movie Ticketing System Design
## 📑 Table of Contents

- [Movie Ticketing System Design](#movie-ticketing-system-design)
- [Part A: Requirement Analysis](#part-a-requirement-analysis)
  - [1. Functional Requirements (FR)](#1-functional-requirements-fr)
  - [2. Non-Functional Requirements (NFR)](#2-non-functional-requirements-nfr)
- [Part B — Noun–Verb Analysis](#part-b--nounverb-analysis)
  - [Part 1: Noun Analysis](#part-1-noun-analysis)
  - [Part 2: Verb Analysis — Mapping Verbs to Methods](#part-2-verb-analysis--mapping-verbs-to-methods)
  - [Part 3: Final Class List with Methods](#part-3-final-class-list-with-methods)
- [Part C — The Classes needed](#part-c--the-classes-needed)
  - [Core Entity Responsibility Summary](#core-entity-responsibility-summary)
  - [Behaviour / Service Responsibility Summary](#behaviour--service-responsibility-summary)
- [Part D — Class Diagram](#part-d--class-diagram)
- [Part E — Sequence Diagram](#part-e--sequence-diagram)
- [Part F — OOP, SOLID & Software Design](#part-f--oop-solid--software-design)
  - [1. OOP Concepts Used](#1-oop-concepts-used)
  - [2. SOLID Principles Used](#2-solid-principles-used)
  - [3. Software Design Patterns Used](#3-software-design-patterns-used)
- [Part G — Modular Code](#part-g--modular-code)
# Demo of whole workflow & edge case handling

<img width="800" height="430" alt="screenrecording-2026-09-13-23-03-47_z3XF1hjn-ezgif com-video-to-gif-converter" src="https://github.com/user-attachments/assets/5d37bb9c-641f-4087-a777-6a10c40fa8f8" />

---

# Part A: Requirement Analysis

## 1. Functional Requirements (FR)
Functional requirements define the specific behaviors and features the system must support. They are written precisely enough to be directly testable during the demo.

| FR No. | Title of FR | Description |
|---|---|---|
| **FR1** | List Movies | The system shall display a list of all movies currently playing, including their title, language, and duration in minutes. |
| **FR2** | List Shows | For a user-selected movie, the system shall display all available shows, including the unique Show ID, Screen number, and start time. |
| **FR3** | Display Seat Layout | For a chosen show, the system shall display the complete seat layout, clearly indicating the current status of every seat as either `AVAILABLE` or `BOOKED`. |
| **FR4** | Book Seats | The system shall allow a user to book one or more seats for a specific show, and shall explicitly reject the request if any selected seat is already `BOOKED` or invalid. |
| **FR5** | Price Calculation | The system shall calculate the total booking amount dynamically based on seat type, strictly applying the pricing rules: SILVER = ₹150, GOLD = ₹250, and PLATINUM = ₹400. |
| **FR6** | Process Payment | The system shall allow payment via UPI, Card, or Cash. If the payment transaction fails or is invalid, the system must NOT confirm the booking and must release the held seats. |
| **FR7** | Print Ticket | Upon successful payment, the system shall generate and print a ticket containing the Booking ID, Movie title, Screen number, Start time, Booked seat numbers, and Total amount. |
| **FR8** | Cancel Booking | The system shall allow a user to cancel an existing confirmed booking using its Booking ID, and shall immediately revert the status of all associated seats back to `AVAILABLE`. |


## 2. Non-Functional Requirements (NFR)
Non-functional requirements define the system's quality attributes, architectural constraints, and design principles as specified in the assignment guidelines.

| NFR No. | Title of NFR | Description |
|---|---|---|
| **NFR1** | Modularity | The system shall be strictly modular, adhering to a "one class per file" architecture without the use of header files, ensuring that each domain entity and service can be maintained independently. |
| **NFR2** | Extensibility (OCP) | The payment subsystem shall be designed using the Open/Closed Principle. Adding a new payment method (e.g., NetBanking) must only require creating a new subclass of `Payment`, without forcing edits to the existing `BookingService` or base `Payment` classes. |
| **NFR3** | Robustness & Validation | The system shall gracefully handle all forms of invalid user input (e.g., non-numeric menu choices, out-of-range IDs, blank strings, non-existent seat numbers) by displaying clear error messages and re-prompting, ensuring the application never crashes or terminates unexpectedly. |
| **NFR4** | Separation of Concerns (SRP) | The system shall enforce strict separation of concerns. Domain classes (like `Booking` or `Show`) shall not contain presentation logic (like printing tickets or formatting console output), delegating those responsibilities entirely to dedicated service classes like `TicketPrinter` and `CinemaMenu`. |

---

# Part B — Noun–Verb Analysis

## Part 1: Noun Analysis

Every noun found in the problem statement is evaluated below. A noun earns a class only if it has its own data and identity. A noun is rejected if it is merely an attribute, a view, a computed value, or a role played by another object.

| Noun Found | Keep as a Class? | Reason |
|---|---|---|
| Movie | Yes | Has its own data and identity: title, language, duration. Multiple movies exist independently. |
| Seat | Yes | Represents one physical seat with a number and type (SILVER/GOLD/PLATINUM). Exists independently of any show. |
| Screen | Yes | Represents one auditorium with a screen number. Owns its collection of seats. Has its own identity. |
| Cinema | Yes | Represents the theatre with a name. Owns its collection of screens. Top-level container object. |
| Show | Yes | Represents one screening of a movie on a screen at a specific time. Owns its ShowSeat objects. Has its own identity (show ID). |
| ShowSeat | Yes | Represents the status (AVAILABLE/BOOKED) of one seat for one specific show. Distinct from the physical Seat because the same seat can be AVAILABLE in one show and BOOKED in another. |
| Customer | Yes | Has its own data: name and phone. A customer can make multiple bookings over time. |
| Booking | Yes | Has its own data and identity: booking ID, associated show, seats, total amount, and status (CONFIRMED/CANCELLED). |
| Payment | Yes | Represents the payment contract. Must be abstract since UPI, Card, and Cash each pay differently. Polymorphism requires a base type. |
| UpiPayment | Yes | A concrete payment method with its own logic (validates UPI ID). Inherits from Payment. |
| CardPayment | Yes | A concrete payment method with its own logic (validates card number). Inherits from Payment. |
| CashPayment | Yes | A concrete payment method with its own logic (checks cash received ≥ amount). Inherits from Payment. |
| PriceCalculator | Yes | Has a clear single responsibility: turning a list of seats into a total amount. Stateless service class. |
| TicketPrinter | Yes | Has a clear single responsibility: formatting and printing a ticket. Printing only, no state mutation. |
| BookingService | Yes | The orchestrator that runs the booking flow end to end. Coordinates Show, ShowSeat, PriceCalculator, Payment, Booking, and TicketPrinter. |
| Menu / CinemaMenu | Yes | Handles the console menu display and all input reading. Separates UI concern from business logic. |
| Seat layout | No | It is a view of a Show's seats, not a thing in itself. It becomes a print/display method on Show. |
| Ticket | No | It is the output produced by TicketPrinter. It carries no state of its own and has no independent lifecycle. It is a formatted printout, not an object. |
| Price | No | A computed value, not an object. It is the return value of PriceCalculator.calculate(). |
| Total amount | No | A calculated number stored as an attribute of Booking. Not an independent object. |
| Seat type (SILVER / GOLD / PLATINUM) | No | An enumeration / attribute of Seat, not a class. Does not have behaviour or independent identity. |
| Seat status (AVAILABLE / BOOKED) | No | An enumeration / attribute of ShowSeat, not a class. Does not have behaviour or independent identity. |
| Booking status (CONFIRMED / CANCELLED) | No | An enumeration / attribute of Booking, not a class. |
| Booking ID | No | A unique identifier generated by a static counter inside Booking. It is an attribute, not an object. |
| Start time | No | An attribute of Show. A string or time value, not an object. |
| Title | No | An attribute of Movie. A string value, not an object. |
| Language | No | An attribute of Movie. A string value, not an object. |
| Duration | No | An attribute of Movie. An integer value, not an object. |
| Name (customer) | No | An attribute of Customer. A string value, not an object. |
| Phone | No | An attribute of Customer. A string value, not an object. |
| Screen number | No | An attribute of Screen. An integer value, not an object. |
| Amount | No | A numeric parameter passed to Payment.pay(). Not an object. |

## Part 2: Verb Analysis — Mapping Verbs to Methods

Every verb found in the problem statement is mapped to the class and method that owns it.

| Verb Found in Problem | Class Responsible | Method |
|---|---|---|
| List movies | BookingService | listMovies() |
| List shows for a movie | BookingService | listShowsForMovie(movieChoice) |
| Display seat layout | Show | displaySeatLayout() |
| See which seats are free | Show | displaySeatLayout() |
| Book seats | BookingService | bookSeats(request, payment) |
| Book a single seat (mark as BOOKED) | ShowSeat | book() |
| Reject an already-booked seat | BookingService | tryAddSeat() checks isBooked() before allowing |
| Price the booking | PriceCalculator | calculate(selectedSeats) |
| Pay | Payment (abstract) | pay(amount) — pure virtual |
| Pay by UPI | UpiPayment | pay(amount) |
| Pay by Card | CardPayment | pay(amount) |
| Pay by Cash | CashPayment | pay(amount) |
| Print a ticket | TicketPrinter | print(booking) |
| Cancel a booking | BookingService | cancelBooking(bookingId) |
| Release seats on cancellation | ShowSeat | release() |
| Generate booking ID | Booking | static nextBookingId increments in constructor |
| Find a show by ID | BookingService | findShow(showId) |
| Find a seat in a show | Show | findShowSeat(seatNumber) |
| Read user input | CinemaMenu | readInteger(), readNonEmptyLine(), readSeatNumbers() |
| Display menu | CinemaMenu | displayMenu() |
| Handle menu choice | CinemaMenu | handleMenuChoice(menuChoice) |
---
## Part 3: Final Class List with Methods

Below is the final list of classes kept after noun analysis, with all their methods derived from the verb analysis.

| Class | Category | Methods |
|---|---|---|
| Movie | Entity | getTitle(), getLanguage(), getDurationMinutes() |
| Seat | Entity | getSeatNumber(), getSeatType() |
| Screen | Entity | getScreenNumber(), addSeat(seat), getSeats(), findSeat(seatNumber) |
| Cinema | Entity | getName(), addScreen(screen), findScreen(screenNumber) |
| ShowSeat | Entity | isAvailable(), isBooked(), book(), release(), getSeatNumber(), getSeatType() |
| Show | Entity | getShowId(), getMovie(), getScreen(), getStartTime(), findShowSeat(seatNumber), displaySeatLayout() |
| Customer | Entity | getName(), getPhone() |
| Booking | Entity | getBookingId(), getShow(), getCustomer(), getSeats(), getTotalAmount(), getPaymentMethodName(), isConfirmed(), markCancelled() |
| Payment | Abstract Service | pay(amount) [pure virtual], getMethodName() [pure virtual] |
| UpiPayment | Concrete Service | pay(amount), getMethodName() |
| CardPayment | Concrete Service | pay(amount), getMethodName() |
| CashPayment | Concrete Service | pay(amount), getMethodName() |
| PriceCalculator | Service | calculate(selectedSeats) [static] |
| TicketPrinter | Service | print(booking) |
| BookingService | Orchestrator | addMovie(movie), addShow(show), getMovieCount(), listMovies(), listShowsForMovie(movieChoice), displaySeatLayout(showId), bookSeats(request, payment), cancelBooking(bookingId) |
| CinemaMenu | CLI | run(), displayMenu(), readMenuChoice(), handleMenuChoice(menuChoice), readSeatNumbers(), readCustomer(), createPayment() |

--- 
# Part C — The Classes needed

### Core Entity Responsibility Summary

| Class | What It Knows | What It Does | What It Must NOT Do |
|---|---|---|---|
| `Movie` | Its title, language, and duration. | Provides movie details to the system. | Must not know about shows, seats, availability, or bookings. |
| `Seat` | Its physical seat number and seat type. | Provides seat identity and category. | Must not know whether it is booked for a particular show. |
| `Screen` | Its screen number and the physical seats it contains. | Owns seats and allows seat lookup. | Must not know movie showtimes or per-show seat status. |
| `Cinema` | Its name and the screens it contains. | Owns screens and allows screen lookup. | Must not manage bookings, payments, or ticket printing. |
| `ShowSeat` | One physical seat and its status for one specific show. | Changes its status between AVAILABLE and BOOKED through controlled methods. | Must not calculate price, process payment, or know booking details. |
| `Show` | The movie, screen, start time, and show-specific seat statuses. | Provides seat layout and show-seat lookup. | Must not own Movie or Screen, and must not process payment. |
| `Customer` | The customer's name and phone number. | Provides customer details for a booking. | Must not modify seats, bookings, or payments. |
| `Booking` | Booking ID, show, customer, seats, amount, status, and payment method name. | Stores confirmed booking state and allows cancellation status update. | Must not process payment, print tickets, or directly release seats. |

---

### Behaviour / Service Responsibility Summary

| Class | What It Knows | What It Does | What It Must NOT Do |
|---|---|---|---|
| `Payment` | Only the payment contract. | Defines the abstract `pay()` and `getMethodName()` interface. | Must not know booking, seat, show, or ticket details. |
| `UpiPayment` | The UPI ID entered by the customer. | Validates UPI details and returns payment success/failure. | Must not confirm booking or modify seats. |
| `CardPayment` | The card number entered by the customer. | Validates card details and returns payment success/failure. | Must not confirm booking or modify seats. |
| `CashPayment` | The cash amount received from the customer. | Checks whether cash received is sufficient and returns payment success/failure. | Must not confirm booking or modify seats. |
| `PriceCalculator` | Seat pricing rules: SILVER ₹150, GOLD ₹250, PLATINUM ₹400. | Calculates total amount from selected seats. | Must not modify seats, shows, bookings, or payments. |
| `TicketPrinter` | Ticket formatting rules. | Prints ticket details from a confirmed booking. | Must not modify booking state or release seats. |
| `BookingService` | Available movies, shows, and confirmed bookings. | Orchestrates listing, booking, payment coordination, and cancellation. | Must not create concrete payment objects or calculate prices directly. |
| `CinemaMenu` | The console menu flow and BookingService interface. | Reads user input and calls BookingService methods. | Must not implement core booking, pricing, payment, or cancellation logic itself. |

---
# Part D — Class Diagram

## Notation used in this diagram

| Relationship | Mermaid Syntax | Meaning |
|---|---|---|
| Composition | `*--` | Filled diamond ◆ — part dies with whole |
| Aggregation | `o--` | Hollow diamond ◇ — part can exist independently |
| Association | `-->` | Open arrow — objects interact |
| Inheritance | `<|--` | Hollow triangle ▷ — child inherits from base |

---

<img width="7894" height="8192" alt="Untitled diagram-2026-09-13-170451" src="https://github.com/user-attachments/assets/34ea1755-e3b2-4da3-b0f1-09234a5c72c8" />

---

# Part E — Sequence Diagram

<img width="8191" height="5998" alt="Untitled diagram-2026-09-13-172035" src="https://github.com/user-attachments/assets/d0cbd0b2-7088-4cfb-b1c6-2aa19f1ac5c8" />

---

## Part F — OOP, SOLID & Software Design

This section explains the object-oriented concepts, SOLID principles, and software design patterns used in the Movie Ticket Booking System.

---

### 1. OOP Concepts Used

| OOP Concept | Where Used | Use Case in This Project |
|---|---|---|
| Encapsulation | `ShowSeat`, `Booking` | Seat status and booking details are kept private. Status changes only through controlled methods such as `book()`, `release()`, and `markCancelled()`, preventing invalid updates. |
| Abstraction | `Payment` | The abstract `Payment` class defines only the payment contract using `pay(amount)`. The booking flow does not need to know how UPI, Card, or Cash payment works internally. |
| Inheritance | `UpiPayment`, `CardPayment`, `CashPayment` | All concrete payment classes inherit from the abstract `Payment` class and provide their own implementation of the `pay()` method. |
| Runtime Polymorphism | `BookingService` using `Payment&` | `BookingService` calls `payment.pay(totalAmount)` through a base `Payment` reference. At runtime, the correct UPI, Card, or Cash implementation is executed. |
| Compile-Time Polymorphism | `PriceCalculator` | Overloaded calculation methods are used to calculate price for selected seats, allowing the same method name to handle different input forms. |
| Static Members | `Booking`, `Show` | Static counters such as `nextBookingId` and `nextShowId` are used to generate unique booking IDs and show IDs across all objects. |
| `this` Keyword | Constructors and member methods | The `this` keyword is used to refer to the current object, especially while assigning member variables and handling object state. |
| Composition | `Cinema` → `Screen`, `Screen` → `Seat`, `Show` → `ShowSeat` | The owning class creates and manages the lifetime of its parts. If a `Cinema`, `Screen`, or `Show` is destroyed, its owned screens, seats, or show seats are also destroyed. |
| Aggregation | `Show` → `Movie`, `Show` → `Screen`, `Booking` → `ShowSeat` | The parent class uses existing objects but does not own them. For example, a `Show` uses a `Movie` and a `Screen`, but they can exist independently of the show. |
| Association | `Customer` → `BookingService` | A customer interacts with `BookingService` to request booking-related operations, but neither object owns the other. |

---

### 2. SOLID Principles Used

| SOLID Principle | Where Applied | Use Case in This Project |
|---|---|---|
| Single Responsibility Principle | `TicketPrinter`, `PriceCalculator`, `Booking`, `BookingService` | Each class has one clear responsibility. `Booking` stores booking data, `PriceCalculator` calculates price, `TicketPrinter` prints tickets, and `BookingService` coordinates the flow. |
| Open/Closed Principle | `Payment` hierarchy | New payment methods such as NetBanking can be added by creating a new subclass of `Payment`. Existing classes like `BookingService` do not need to be modified. |
| Liskov Substitution Principle | `UpiPayment`, `CardPayment`, `CashPayment` | Any subclass of `Payment` can be passed to `BookingService` and used through the same `Payment` interface without special handling or extra setup. |
| Interface Segregation Principle | `Payment` abstract class | The `Payment` contract only forces payment-related behavior. It does not force unrelated methods such as `refund()` on every payment type. |
| Dependency Inversion Principle | `BookingService` depending on `Payment&` | `BookingService` depends on the abstract `Payment` type instead of concrete classes like `UpiPayment` or `CardPayment`. The concrete payment object is supplied from outside. |

---

### 3. Software Design Patterns Used

| Design Pattern | Category | Where Used | Use Case in This Project |
|---|---|---|---|
| Strategy Pattern | Behavioral | `Payment`, `UpiPayment`, `CardPayment`, `CashPayment` | Different payment methods are encapsulated as interchangeable strategies. `BookingService` uses the abstract `Payment` strategy without knowing the concrete payment logic. |
| Simple Factory Pattern | Creational | `CinemaMenu` payment creation | The menu creates the appropriate concrete payment object based on the customer's choice. This keeps object creation separate from the booking orchestration logic. |
| Facade Pattern | Structural | `BookingService` | `BookingService` provides a simple interface for operations such as booking seats and cancelling bookings. It hides the internal coordination between `Show`, `ShowSeat`, `PriceCalculator`, `Payment`, `Booking`, and `TicketPrinter`. |

> Software Design Patterns such as Singleton, Observer, Decorator, and State were intentionally not used because they were not required for this system’s scope.

---

## Part G — Modular Code

```text
├── .gitignore                 # Ignores build output and editor files
├── 01_Movie.cpp               # Stores movie title, language, and duration
├── 02_Seat.cpp                # Stores physical seat number and type
├── 03_Screen.cpp              # Owns seats inside one auditorium
├── 04_Cinema.cpp              # Owns screens for one cinema
├── 05_Show.cpp                # Links movie, screen, time, and show seats
├── 06_ShowSeat.cpp            # Tracks one seat status for one show
├── 07_Customer.cpp            # Stores customer name and phone
├── 08_Booking.cpp             # Stores booking details and status
├── 09_Payment.cpp             # Defines abstract payment contract
├── 11_PriceCalculator.cpp     # Calculates total seat price
├── 12_TicketPrinter.cpp       # Formats and prints ticket
├── 13_BookingService.cpp      # Orchestrates booking and cancellation flow
├── 14_UpiPayment.cpp          # Implements UPI payment logic
├── 15_CardPayment.cpp         # Implements card payment logic
├── 16_CashPayment.cpp         # Implements cash payment logic
├── 17_CinemaMenu.cpp          # Handles console menu and input
├── build.sh                   # Compiles and builds the project
├── Common.cpp                 # Holds shared constants, enums, and helpers
├── main.cpp                   # Starts program and loads demo data
└── README.md                  # Explains project setup and usage
```
