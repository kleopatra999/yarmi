Yet Another [RMI](http://en.wikipedia.org/wiki/Java_remote_method_invocation)
=========

RMI constructor using C++ preprocessor

### Для чего "оно" нужно, и с чем "его" едят?

Представьте, что вам нужно реализовать взаимодействие клиент-сервер, по принципу [RMI](http://en.wikipedia.org/wiki/Java_remote_method_invocation). Что для этого нужно, вы наверняка знаете:

1. на вызывающей стороне описать функцию с сигнатурой, соответствующей вызываемой процедуре
2. сериализовать аргументы вызова
3. на вызываемой стороне описать вызываемую процедуру с сигнатурой, соответствующей вызывающей стороне
4. десериализовать пришедшие данные
5. подставить их в вызов соответствующей процедуры
6. произвести вызов соответствующей процедуры

Все мы это делали множество раз, и во всем этом нет ничего сложного, с одной лишь оговоркой - когда кол-во процедур и их аргументов исчисляется единицами ;)

#### *И все же, для чего "оно" нужно?*
Терпение, друг мой... Сейчас все будет.

Описываем имена объектов-обработчиков, и имена процедур в синтаксисе препроцессора:
```cpp
YARMI_CONSTRUCT(
    (yarmi), // неймспейс, в котором будет сгенерен 'client_invoker'
    client_invoker, // имя класса клиентского инвокера
    (ping, on_ping, (std::string)), // процедура: 1)имя вызывающей процедуры, 2)имя вызываемой процедуры, 3)список аргументов процедуры
    (yarmi), // неймспейс в котором будет сгенерен 'server_invoker'
    server_invoker, // имя класса серверного инвокера
    (pong, on_pong, (std::string)) // процедура: 1)имя вызывающей процедуры, 2)имя вызываемой процедуры, 3)список аргументов процедуры
)
```
Всю остальную работу, YARMI(`YARMI_CONSTRUCT()`) сделает за нас!

#### *Что именно "оно" сделает?*
[Ну вот же](https://github.com/niXman/yarmi/blob/master/examples/echo/protocol-preprocessed.hpp):

1. Метод `client_invoker::ping()` соответствует описанному нами в макросе `YARMI_CONSTRUCT()`
2. В методе `client_invoker::invoke()`, в `switch`е, производится десериализация аргументов, подставновка их в вызов, и, собственно, вызов `client_invoker::on_pong()`.
3. В методе `server_invoker::invoke()`, в `switch`е, происходит тоже самое с тем лишь отличием, что проиходит вызов метода `server_invoker::on_ping()`. Сервер пошлет ответ клиенту, вызвав этот метод. Т.е. порядок вызовов такой: `client_invoker::ping()` -> `server_invoker::on_ping()` -> `server_invoker::pong()` -> `client_invoker::on_pong()`.
4. Вам не нужно более заботиться ни о соответствии сигнатур, ни о идентификаторах, ни о сериализаторах/десериализаторах.

В добавок ко всему этому, одним немаловажным плюсом является и то, что сейчас невероятно сложно допустить ошибку. И, даже если мы ее допустим - узнаем мы об этом во время собрки проекта.

Как вы могли заметить, классы `client_invoker` и `server_invoker` принимают два параметра. Первый их низ - класс в котором реализованы вызываемые процедуры, второй - класс в котором реализован метод `send(yas::shared_buffer buf)`. (в качестве сериализации/десериализации используется другой мой проект - [YAS](https://github.com/niXman/yas))

#### *Как предполагается "это" использовать в конечном счете?*
Опять же - все просто!

```cpp
// client side
struct client: yarmi::client_base, yarmi::client_invoker<client> {
    client(boost::asio::io_service &ios)
        :yarmi::client_base(ios)
        ,yarmi::client_invoker<client>(*this, *this)
    {
        // запрос к серверу
        ping("Hello, server!");
    }
    
    // на ответ сервера вызывается этот метод
    void on_pong(const std::string &msg) {
        std::cout << "server ask:\"" << msg << "\"" << std::endl;
        // из этого метода мы снова зовем сервер
        ping("Hello, server, again!");
    }
};

// server side
struct session: yarmi::session_base, yarmi::server_invoker<session> {
    session(boost::asio::io_service &ios)
        :yarmi::session_base(ios)
        ,yarmi::server_invoker<session>(*this, *this)
    {}
    
    // вызывается при запросе клиента
    void on_ping(const std::string &msg) {
        // отвечаем
        if ( msg.find("again") == std::string::npos )
            pong("Hello, client!");
        else
            pong("Hello, client, again!");
    }
};
```

Еще одна, немаловажная для интернет-сервисов плюшка - возможность перегрузки процедур. Это позволит нам создавать новые версии API нашего сервиса, и при этом, не требуя обновления наших клиентов.
т.е., к примеру, мы может сделать такое:
```cpp
YARMI_CONSTRUCT(
    (yarmi),
    client_invoker,
    (ping, on_ping, (std::string)),
    (ping, on_ping, (std::string, std::string)),
    (yarmi),
    server_invoker,
    (pong, on_pong, (std::string))
    (pong, on_pong, (std::string, std::string))
)
```
Таким образом, клиент может может вызывать как `ping("message")` так и `ping("message", "user name")`. И, соответственно, сервер может слать два типа ответа. И снова повторюсь: ничего руками делать не нужно! соответствие `ID`ов процедур и их версий - это забота yarmi!

### Более сложный пример
```cpp
YARMI_CONSTRUCT(
	(yarmi),        // client invoker namespace
	client_invoker, // name of the client invoker
	(registration , on_registration, (std::string)                           ) // username
	(activation   , on_activation  , (std::string, std::string, std::string) ) // registration key : username : password
	(login        , on_login       , (std::string, std::string)              ) // username : password
	(logout       , on_logout      , ()                                      ) // without args
	(users_online , on_users_online, ()                                      ) // without args
	(users_online , on_users_online, (std::string)                           ) // substring of username
	,
	(yarmi),        // server invoker namespace
	server_invoker, // name of the server invoker
	(registration , on_registration, (std::string, std::string)              ) // message : registration key
	(activation   , on_activation  , (std::string)                           ) // message
	(login        , on_login       , (std::string)                           ) // message
	(logout       , on_logout      , (std::string)                           ) // message
	(users_online , on_users_online, (std::vector<std::string>)              ) // vector of usernames
);
```
Результат [тут](https://github.com/niXman/yarmi/blob/master/examples/chat/protocol-preprocessed.hpp).

Как вы могли заметить, имена процедур на стороне сервера и клиента пересекаются. Да, в этом нет ничего необычного ;)


TODO
=========

1. Описать спецификацию (хоть она и проста больше некуда)
2. Возможность использовать собственный алокатор
3. Добавить возможность выбора протокола (tcp/udp)
4. Добавить возможность использовать Unix domain socket
