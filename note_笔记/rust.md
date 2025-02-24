https://blog.csdn.net/tyyj90/article/details/142074351



面向对象编程（Object-Oriented Programming，OOP）是一种对程序进行建模方式。对象（Object）作为一个编程概念来源于 20 世纪 60 年代的 Simula 编程语言。这些对象影响了 Alan Kay 的编程架构，该架构中对象之间互相传递消息。他在 1967 年创造了面向对象编程 （object-oriented programming）这个术语。关于 OOP 是什么有很多相互矛盾的定义；在一些定义下，Rust 是面向对象的；在其他定义下，Rust 不是。
一、Rust 面向对象

The Gang of Four (“四人帮”)，是这样定义面向对象编程的：

面向对象的程序由对象组成。一个对象包含数据和操作这些数据的过程。这些过程通常被称为方法或操作。

在这个定义下，Rust 是面向对象的：结构体和枚举包含数据而 impl 块提供了在结构体和枚举之上的方法。虽然带有方法的结构体和枚举并不被称为 对象，但是它们提供了与对象相同的功能，参考 The Gang of Four 中对象的定义。

通常来说面向对象的三个基本特征是：封装、继承和多态。
1.1 封装

封装（encapsulation）：一个对象的实现细节对使用该对象的代码不可访问。因此，对象交互的唯一方式是通过其公共 API；使用对象的代码不应能直接触及对象的内部并改变数据或行为。这使得程序员能够更改和重构一个对象的内部实现，而无需改变使用该对象的代码。

虽然 Rust 语言本身并不支持传统的面向对象编程中的类（class）和继承（inheritance），但它提供了一些特性，允许开发者以类似面向对象的方式进行封装和抽象。以下是 Rust 中体现封装特性的一个例子：

```rust
// 定义一个结构体 `Rectangle`，相当于面向对象中的类

struct Rectangle {
    width: u32,
    height: u32,
}

// 为 `Rectangle` 结构体实现方法，这些方法封装了与 `Rectangle` 相关的操作
impl Rectangle {
    // 关联函数，用于创建一个新的 `Rectangle`
    fn new(width: u32, height: u32) -> Rectangle {
        Rectangle { width, height }
    }

    // 实例方法，用于计算矩形的面积
    fn area(&self) -> u32 {
        self.width * self.height
    }

    // 实例方法，用于计算矩形的周长
    fn perimeter(&self) -> u32 {
        (self.width + self.height) * 2
    }

    // 实例方法，用于设置矩形的宽度
    fn set_width(&mut self, width: u32) {
        self.width = width;
    }

    // 实例方法，用于设置矩形的高度
    fn set_height(&mut self, height: u32) {
        self.height = height;
    }
}

fn main() {
    // 创建一个矩形实例
    let mut rect = Rectangle::new(10, 20);

    // 调用实例方法来获取面积和周长
    println!("The area of the rectangle is {}", rect.area());
    println!("The perimeter of the rectangle is {}", rect.perimeter());

    // 修改矩形的宽度和高度
    rect.set_width(15);
    rect.set_height(25);

    // 再次调用实例方法来获取更新后的面积和周长
    println!("The new area of the rectangle is {}", rect.area());
    println!("The new perimeter of the rectangle is {}", rect.perimeter());
}
```
运行结果
```
The area of the rectangle is 200
The perimeter of the rectangle is 60
The new area of the rectangle is 375
The new perimeter of the rectangle is 80
```
    结构体（Struct）：Rectangle 结构体类似于面向对象编程中的类，它定义了矩形的数据结构，包括宽度和高度。

    方法（Method）：impl 块为 Rectangle 结构体实现了方法。这些方法提供了对 Rectangle 实例的操作，如创建新实例、计算面积和周长、设置宽度和高度。

    封装（Encapsulation）：在 Rust 中，封装是通过将数据（结构体字段）和操作这些数据的函数（方法）组合在一起实现的。在这个例子中，Rectangle 的字段 width 和 height 是私有的，只能通过结构体提供的方法来访问和修改，这保护了数据不被外部代码直接访问。

    关联函数（Associated Function）：new 是一个关联函数，它不作用于结构体的实例，而是用来创建结构体的新实例。

    实例方法（Instance Method）：area、perimeter、set_width 和 set_height 是实例方法，它们作用于 Rectangle 的实例，并可以访问和修改实例的状态。

1.2 继承

继承（Inheritance）是一种机制：一个对象可以从另一个对象的定义中继承元素，从而获得父对象的数据和行为，无需再次定义。

如果一种语言必须具有继承才能被认为是面向对象语言，那么 Rust 不是其中之一。Rust 不支持定义一个结构体时继承父结构体的字段和方法，除非使用宏。

然而，如果您习惯于在编程过程中使用继承，那么根据运用继承的原因，Rust 提供了其他解决方案。

选择继承有两个主要的原因。其一是代码复用：您可以为一种类型实现特定的行为，继承可将其复用到不同的类型上。在 Rust 代码中可以使用默认 trait 方法实现来进行有限的代码复用。其二与类型系统有关：子类型可以用于父类型被使用的地方。这也被称为多态（polymorphism）：如果多个对象共享某些特征，可以在运行时将它们互相替代。

在 Rust 中，可以通过 trait 和默认方法（default methods）来实现类似于面向对象编程中的继承特性。默认方法允许 trait 为某些方法提供默认实现，这些方法可以在实现该 trait 的类型中被覆盖。通过使用 trait 和默认方法，Rust 允许开发者以一种类似于面向对象编程的方式实现代码复用和方法覆盖，尽管它没有传统意义上的类继承机制。这种方式使得 Rust 既保持了类型安全，又提供了灵活的代码组织方式。

以下是一个使用 trait 和默认方法实现代码复用，并展示如何覆盖默认方法的例子：
```rust
// 定义一个 trait `Animal`
trait Animal {
    fn make_sound(&self);
    // 默认方法实现，可以被覆盖
    fn description(&self) {
        println!("This animal makes a sound.");
    }
}

// 定义一个结构体 `Dog`，它实现了 `Animal` trait
struct Dog;

impl Animal for Dog {
    fn make_sound(&self) {
        println!("Woof!");
    }

    // 覆盖默认的 `description` 方法
    fn description(&self) {
        println!("A dog is a loyal companion that says Woof!");
    }
}

// 定义另一个结构体 `Cat`，它也实现了 `Animal` trait
struct Cat;

impl Animal for Cat {
    fn make_sound(&self) {
        println!("Meow!");
    }

    // 使用默认的 `description` 方法，不进行覆盖
}

fn main() {
    let dog = Dog;
    let cat = Cat;

    dog.make_sound();
    dog.description();

    cat.make_sound();
    cat.description();
}
```
运行结果
```
Woof!
A dog is a loyal companion that says Woof!
Meow!
This animal makes a sound.
```
    Trait（接口）：Animal trait 定义了所有动物应该实现的方法，包括 make_sound 和一个默认方法 description。

    默认方法（Default Methods）：description 是 Animal trait 中的一个默认方法，它提供了一个通用的描述。在实现该 trait 的类型中，这个方法可以被覆盖以提供更具体的描述。

    实现（Implementation）：Dog 和 Cat 结构体都实现了 Animal trait。Dog 结构体覆盖了默认的 description 方法，提供了一个更具体的描述，而 Cat 结构体则使用了默认的 description 方法实现。

    覆盖默认方法（Overriding Default Methods）：在 Dog 的实现中，description 方法被覆盖，以提供关于 Dog 的更具体的信息。这类似于面向对象编程中的继承和方法重写。

    代码复用：通过 trait 和默认方法，Rust 允许代码复用。在 Cat 的实现中，由于没有覆盖 description 方法，它直接使用了 Animal trait 提供的默认实现。

    多态性（Polymorphism）：Rust 的 trait 允许多态性，这意味着不同的类型可以实现同一个 trait，并可以覆盖其默认方法。

作为一种语言设计的解决方案，继承在许多新的编程语言中逐渐不被青睐，因为它经常有分享过多代码的风险。子类不应总是共享父类的所有特征，但是继承始终如此。它还引入了在子类上调用方法的可能性，这些方法可能没有意义，或因为方法不适用于子类而导致错误。此外，一些语言只允许单一继承（意味着子类只能从一个类继承），进一步限制了程序设计的灵活性。
1.3 多态

对很多人来说，多态性与继承同义。但它实际上是一个更广义的概念，指的是可以处理多种类型数据的代码。对继承而言，这些类型通常是子类。 Rust 使用泛型来抽象不同可能的类型，并通过 trait bounds 来约束这些类型所必须提供的内容。这有时被称为 bounded parametric polymorphism。

以下是一个使用泛型和 trait bounds 来实现多态性的例子：
```rust
// 定义一个 trait `Animal`，它包含一个方法 `make_sound`
trait Animal {
    fn make_sound(&self);
}

// 定义一个结构体 `Dog`，它实现了 `Animal` trait
struct Dog;

impl Animal for Dog {
    fn make_sound(&self) {
        println!("Woof!");
    }
}

// 定义另一个结构体 `Cat`，它也实现了 `Animal` trait
struct Cat;

impl Animal for Cat {
    fn make_sound(&self) {
        println!("Meow!");
    }
}

// 使用泛型和 trait bounds 来定义一个函数，它可以接收任何实现了 `Animal` trait 的类型
fn animal_sound<T: Animal>(animal: &T) {
    animal.make_sound();
}

fn main() {
    let dog = Dog;
    let cat = Cat;

    // 调用 `animal_sound` 函数，传入不同类型的引用
    animal_sound(&dog);
    animal_sound(&cat);
}
```
运行结果
```
Woof!
Meow!
```
    Trait（接口）：Animal trait 定义了一个 make_sound 方法，所有实现这个 trait 的类型都必须提供这个方法的具体实现。

    实现（Implementation）：Dog 和 Cat 结构体都实现了 Animal trait。这意味着它们都提供了 make_sound 方法的具体实现，分别打印 “Woof!” 和 “Meow!”。

    泛型（Generics）：animal_sound 函数使用泛型 T 来表示它可以接收任何类型的参数。

    Trait Bounds（特质约束）：T: Animal 是一个 trait bound，它约束了泛型 T 必须实现了 Animal trait。这意味着 animal_sound 函数可以接受任何实现了 Animal trait 的类型的引用。

    多态性（Polymorphism）：通过泛型和 trait bounds，animal_sound 函数可以处理多种实现了 Animal trait 的类型。在运行时，Rust 会根据传入的具体类型调用相应的 make_sound 方法，这体现了多态性。

    类型安全（Type Safety）：Rust 的类型系统确保了只有实现了 Animal trait 的类型才能作为 animal_sound 函数的参数，这保证了类型安全。

二、trait 对象

trait 对象指向一个实现了我们指定 trait 的类型的实例，以及一个用于在运行时查找该类型的 trait 方法的表。我们通过指定某种指针来创建 trait 对象，例如 & 引用或 Box<T> 智能指针，还有 dyn 关键字，以及指定相关的 trait。我们可以使用 trait 对象代替泛型或具体类型。任何使用 trait 对象的位置，Rust 的类型系统会在编译时确保任何在此上下文中使用的值会实现其 trait 对象的 trait。如此便无需在编译时就知晓所有可能的类型。

之前提到过，Rust 刻意不将结构体与枚举称为 “对象”，以便与其他语言中的对象相区别。在结构体或枚举中，结构体字段中的数据和 impl 块中的行为是分开的，不同于其他语言中将数据和行为组合进一个称为对象的概念中。trait 对象将数据和行为两者相结合，从这种意义上说则其更类似其他语言中的对象。不过 trait 对象不同于传统的对象，因为不能向 trait 对象增加数据。trait 对象并不像其他语言中的对象那么通用：其具体的作用是允许对通用行为进行抽象。

当对泛型使用 trait bound 时编译器所执行的单态化处理：编译器为每一个被泛型类型参数代替的具体类型生成了函数和方法的非泛型实现。单态化产生的代码在执行静态分发（static dispatch）。静态分发发生于编译器在编译时就知晓调用了什么方法的时候。这与动态分发 （dynamic dispatch）相对，这时编译器在编译时无法知晓调用了什么方法。在动态分发的场景下，编译器会生成负责在运行时确定该调用什么方法的代码。

当使用 trait 对象时，Rust 必须使用动态分发。编译器无法知晓所有可能用于 trait 对象代码的类型，所以它也不知道应该调用哪个类型的哪个方法实现。为此，Rust 在运行时使用 trait 对象中的指针来知晓需要调用哪个方法。动态分发也阻止编译器有选择的内联方法代码，这会相应的禁用一些优化。

以下是使用 trait 对象和动态分发的示例：
```rust
// 定义一个 trait `Animal`
trait Animal {
    fn make_sound(&self);
}

// 实现 `Animal` trait 给 `Dog`
struct Dog;

impl Animal for Dog {
    fn make_sound(&self) {
        println!("Woof!");
    }
}

// 实现 `Animal` trait 给 `Cat`
struct Cat;

impl Animal for Cat {
    fn make_sound(&self) {
        println!("Meow!");
    }
}

// 定义一个使用 trait 对象的函数，这里使用了动态分发
fn make_animal_sound(animal: &dyn Animal) {
    animal.make_sound();
}

fn main() {
    let dog = Dog;
    let cat = Cat;

    // 将不同类型的引用转换为 trait 对象
    let dog_trait: &dyn Animal = &dog;
    let cat_trait: &dyn Animal = &cat;

    // 调用函数，使用 trait 对象
    make_animal_sound(dog_trait);
    make_animal_sound(cat_trait);
}
```
运行结果
```
Woof!
Meow!
```
在这个例子中，make_animal_sound 函数接受一个 &dyn Animal 类型的参数，这是一个对实现了 Animal trait 的类型的引用。dyn Animal 是一个 trait 对象，它使用了动态分发来调用 make_sound 方法。

    Trait 对象：&dyn Animal 是一个 trait 对象，它允许存储实现了 Animal trait 的任何类型的引用。

    动态分发：当使用 &dyn Animal 调用 make_sound 方法时，Rust 需要在运行时确定应该调用哪个具体的 make_sound 实现。这是通过在编译时将方法调用转换为一个虚函数表（vtable）索引来实现的。

    多态性：使用 trait 对象可以在运行时处理不同的类型，这体现了多态性。

    内存分配：由于使用了 &dyn，Rust 需要在堆上分配内存来存储虚函数表的指针，这与静态分发相比增加了一些运行时开销。

    灵活性：Trait 对象提供了极大的灵活性，允许开发者编写可以处理多种类型的代码，而不需要关心这些类型的具体实现细节。

使用 trait 对象和动态分发是 Rust 中实现多态性的一种方式，但它通常不推荐作为首选，因为 Rust 的泛型和 trait bounds 通常可以提供足够的灵活性，同时保持更好的性能和类型安全。Trait 对象在需要动态或运行时类型信息时非常有用。
三、面向对象设计模式

下面将提供几个面向对象设计模式的例子。
3.1 简单工厂模式
```rust
trait Animal {
    fn make_sound(&self);
}

struct Dog;
struct Cat;

impl Animal for Dog {
    fn make_sound(&self) {
        println!("Woof!");
    }
}

impl Animal for Cat {
    fn make_sound(&self) {
        println!("Meow!");
    }
}

struct AnimalFactory;

impl AnimalFactory {
    fn create_animal(kind: &str) -> Box<dyn Animal> {
        match kind {
            "dog" => Box::new(Dog),
            "cat" => Box::new(Cat),
            _ => panic!("Unknown animal"),
        }
    }
}

fn main() {
    let animal = AnimalFactory::create_animal("dog");
    animal.make_sound();
}
```
运行结果
```
Woof!
```
    Animal trait 定义了一个 make_sound 方法，所有动物类型都必须实现这个方法。
    Dog 和 Cat 结构体实现了 Animal trait。
    AnimalFactory 结构体提供了一个 create_animal 方法，根据传入的字符串返回一个实现了 Animal trait 的对象。这里使用了 Box<dyn Animal> 来创建一个 trait 对象，允许存储任何实现了 Animal trait 的类型。
    main 函数中，我们使用 AnimalFactory 创建了一个 Dog 对象，并调用了它的 make_sound 方法。

3.2 单例模式
```rust
use std::sync::Mutex;
use lazy_static::lazy_static;

struct Singleton {
    value: i32,
}

impl Singleton {
    fn new() -> Singleton {
        Singleton { value: 0 }
    }

    fn value(&self) -> i32 {
        self.value
    }

    fn set_value(&mut self, value: i32) {
        self.value = value;
    }
}

lazy_static! {
    static ref INSTANCE: Mutex<Singleton> = Mutex::new(Singleton::new());
}

fn main() {
    let mut singleton = INSTANCE.lock().unwrap();
    println!("Singleton value: {}", singleton.value());
    singleton.set_value(42);
    println!("Singleton value after set: {}", singleton.value());
}
```
运行结果
```
Singleton value: 0
Singleton value after set: 42
```
    使用 lazy_static 创建了一个线程安全的 Mutex<Singleton> 包装的单例。
    Singleton 结构体有一个 value 字段，以及获取和设置这个值的方法。
    main 函数中，我们通过 lock 方法获取对单例的访问，并打印和修改它的 value。

3.3 策略模式
```rust
trait Strategy {
    fn execute(&self, a: i32, b: i32) -> i32;
}

struct Add;
struct Subtract;

impl Strategy for Add {
    fn execute(&self, a: i32, b: i32) -> i32 {
        a + b
    }
}

impl Strategy for Subtract {
    fn execute(&self, a: i32, b: i32) -> i32 {
        a - b
    }
}

struct Context {
    strategy: Box<dyn Strategy>,
}

impl Context {
    fn new(strategy: Box<dyn Strategy>) -> Context {
        Context { strategy }
    }

    fn execute_strategy(&self, a: i32, b: i32) -> i32 {
        self.strategy.execute(a, b)
    }
}

fn main() {
    let add_strategy = Add;
    let subtract_strategy = Subtract;

    let context_add = Context::new(Box::new(add_strategy));
    let context_subtract = Context::new(Box::new(subtract_strategy));

    println!("Add: 10 + 5 = {}", context_add.execute_strategy(10, 5));
    println!("Subtract: 10 - 5 = {}", context_subtract.execute_strategy(10, 5));
}
```
运行结果
```
Add: 10 + 5 = 15
Subtract: 10 - 5 = 5
```
    Strategy trait 定义了一个 execute 方法，不同的策略将提供不同的实现。
    Add 和 Subtract 结构体实现了 Strategy trait，分别代表加法和减法操作。
    Context 结构体持有一个 Box<dyn Strategy> 类型的字段，允许在运行时改变使用的策略。
    main 函数中，我们创建了两个 Context 对象，分别使用加法和减法策略，并执行相应的操作。

3.4 观察者模式
```rust
// 定义观察者 trait，所有观察者必须实现 update 方法。
trait Observer {
    // 当观察者需要更新时，调用此方法。
    fn update(&self, data: &str);
}

// Subject 结构体包含一个观察者列表和一个状态。
struct Subject<'a> {
    observers: Vec<&'a dyn Observer>, // 观察者列表，使用生命周期 'a 来确保 Subject 存活时观察者也存活。
    state: String,                    // Subject 的状态。
}

// Subject 的实现。
impl<'a> Subject<'a> {
    // 创建一个新的 Subject 实例。
    fn new(state: String) -> Self {
        Self {
            observers: Vec::new(),
            state,
        }
    }

    // 将观察者添加到 Subject 的观察者列表中。
    fn attach(&mut self, observer: &'a dyn Observer) {
        self.observers.push(observer);
    }

    // 从 Subject 的观察者列表中移除观察者。
    // 使用 std::ptr::eq 来比较观察者的指针，因为观察者是 trait 对象。
    fn detach(&mut self, observer: &dyn Observer) {
        self.observers.retain(|o| !std::ptr::eq(*o, observer));
    }

    // 通知所有观察者状态已更新。
    fn notify(&self) {
        for o in &self.observers {
            o.update(&self.state);
        }
    }

    // 设置 Subject 的状态，并通知所有观察者状态已更新。
    fn set_state(&mut self, state: String) {
        self.state = state;
        self.notify();
    }
}

// 具体的观察者实现。
struct ConcreteObserver {
    name: String, // 观察者的名称。
}

// 为 ConcreteObserver 实现 Observer trait。
impl Observer for ConcreteObserver {
    fn update(&self, data: &str) {
        println!("{} received data: {}", self.name, data);
    }
}

// main 函数，程序的入口点。
fn main() {
    let mut subject = Subject::new("initial data".to_string()); // 创建一个新的 Subject。
    let observer1 = ConcreteObserver {
        name: "Observer 1".to_string(),
    }; // 创建第一个观察者。
    let observer2 = ConcreteObserver {
        name: "Observer 2".to_string(),
    }; // 创建第二个观察者。

    subject.attach(&observer1); // 将 observer1 添加到 Subject 的观察者列表。
    subject.attach(&observer2); // 将 observer2 添加到 Subject 的观察者列表。

    subject.set_state("updated_data".to_string()); // 更新 Subject 的状态，并通知所有观察者。

    subject.detach(&observer2); // 从 Subject 的观察者列表中移除 observer2。

    subject.set_state("Again updated data".to_string()); // 再次更新 Subject 的状态，并通知剩余的观察者。

    subject.detach(&observer1); // 从 Subject 的观察者列表中移除 observer1。
}
```
运行结果
```
Observer 1 received data: updated_data
Observer 2 received data: updated_data
Observer 1 received data: Again updated data
```
    Observer Trait: 定义了一个 Observer trait，它要求实现一个 update 方法，该方法接受一个字符串引用作为参数。

    Subject 结构体: 包含一个观察者列表和一个状态。观察者列表使用 Vec 存储对 Observer trait 对象的引用，并且有一个生命周期参数 'a 来确保观察者在 Subject 存活期间也存活。

    Subject 实现: 包括创建新的 Subject 实例、添加和移除观察者、通知所有观察者状态更新以及设置新状态并通知观察者的方法。

    ConcreteObserver 结构体: 定义了一个具体的观察者，包含一个名称字段。

    ConcreteObserver 实现: 为 ConcreteObserver 实现了 Observer trait 的 update 方法，当观察者接收到更新通知时，会打印出观察者的名称和接收到的数据。

    main 函数: 创建 Subject 和两个 ConcreteObserver 实例，将它们添加到 Subject 的观察者列表中，然后更新状态并观察输出结果。最后，从观察者列表中移除观察者，并再次更新状态以展示观察者已不再接收通知。

3.5 建造者模式

建造者模式（Builder Pattern）是一种常用的设计模式，用于创建复杂对象的实例，同时确保对象的创建过程是灵活和可扩展的。建造者模式通过将对象的构建过程与对象的表示分离，使得构建过程更加清晰和易于管理。

以下是一个简单的 Rust 建造者模式示例，用于创建一个 Person 结构体的实例：
```rust
// 定义 Person 结构体
struct Person {
    name: String,
    age: u8,
    job: Option<String>,
}

// 定义 PersonBuilder 结构体，用于构建 Person 实例
struct PersonBuilder {
    name: String,
    age: u8,
    job: Option<String>,
}

impl PersonBuilder {
    // 创建一个新的 PersonBuilder 实例
    fn new(name: String) -> Self {
        PersonBuilder {
            name,
            age: 0,
            job: None,
        }
    }

    // 设置 Person 的年龄
    fn age(mut self, age: u8) -> Self {
        self.age = age;
        self
    }

    // 设置 Person 的职业
    fn job(mut self, job: String) -> Self {
        self.job = Some(job);
        self
    }

    // 构建并返回 Person 实例
    fn build(self) -> Person {
        Person {
            name: self.name,
            age: self.age,
            job: self.job,
        }
    }
}

fn main() {
    // 使用 PersonBuilder 创建一个 Person 实例
    let person = PersonBuilder::new("Alice".to_string())
        .age(30)
        .job("Engineer".to_string())
        .build();

    println!("Name: {}, Age: {}, Job: {:?}", person.name, person.age, person.job);
}
```
运行结果
```
Name: Alice, Age: 30, Job: Some("Engineer")
```
    定义 Person 结构体：这是一个简单的结构体，包含姓名、年龄和职业（可选）。

    定义 PersonBuilder 结构体：这是一个用于构建 Person 实例的辅助结构体。它包含与 Person 相同的字段，但职业字段是 Option<String> 类型，表示职业可以是可选的。

    实现 PersonBuilder：
        new 方法：创建一个新的 PersonBuilder 实例，初始化姓名字段，年龄和职业默认为 0 和 None。
        age 方法：设置 PersonBuilder 的年龄字段，并返回自身，以便链式调用。
        job 方法：设置 PersonBuilder 的职业字段，并返回自身，以便链式调用。
        build 方法：构建并返回一个 Person 实例。

    在 main 函数中使用：通过链式调用 new、age 和 job 方法，构建一个 Person 实例，并打印其属性。

参考链接

    Rust 官方网站：https://www.rust-lang.org/zh-CN
    Rust 官方文档：https://doc.rust-lang.org/
    Rust Play：https://play.rust-lang.org/
    《Rust 程序设计语言》
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
                        
原文链接：https://blog.csdn.net/tyyj90/article/details/142074351