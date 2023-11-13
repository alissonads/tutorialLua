#include <iostream>
#include <lua5.4/lua.hpp>
#include <new>

//int main() {
//    lua_State *L = luaL_newstate();
//
//    luaL_openlibs(L);
//    if (luaL_dostring(L, "print('Lua: Hello world')") != 0) {
//        fprintf(stderr, "Erro de sintaxe %s\n", lua_tostring(L, -1));
//        return 1;
//    }
//    lua_close(L);
//
//    return 0;
//}

int main() {
    {
        // Lua interpreter instance
        lua_State *L = luaL_newstate();
        // loads the standard Lua libraries
        luaL_openlibs(L);
        // execute a Lua string, second argument is the Lua code that will be executed
        // if return is non-zero, syntax error
        if (luaL_dostring(L, "print('Lua: Hello world')") != 0) {
            fprintf(stderr, "Syntax error %s\n", lua_tostring(L, -1));
            return 1;
        }
        // closes the lua interpreter and releases the resources
        lua_close(L);
    }
    {
        lua_State *L = luaL_newstate();
        // execute a Lua string, second argument is the Lua code that will be executed
        luaL_dostring(L, "x = 7");
        lua_getglobal(L, "x");
        // used to extract the value from the Lua stack
        // the second parameter the index of the variable x on the Lua stack
        lua_Number x = lua_tonumber(L, 1);

        printf("lua_Number x: %d\n", (int) x);

        lua_close(L);
    }

    // types
    // nil
    // boolean
    // light userdata
    // number
    // string
    // table x = {"foo", "bar"}
    // function
    // userdata
    // thread
    {
        lua_State* L = luaL_newstate();
        // adds a value to the Lua stack
        lua_pushnumber(L, 10);
        lua_pushnumber(L, 11);
        lua_pushnumber(L, 12);

        // number 10 - index 1
        // number 11 - index 2
        // number 12 - index 3

        // used to extract the value from the Lua stack
        // the second parameter the index of the variable x on the Lua stack
        lua_Number x = lua_tonumber(L, 2); // ultimo parâmetro o indice onde o numero foi adicionado
        printf("lua_Number x: %d\n", (int) x); // 11

        // number 10 - index -3
        // number 11 - index -2
        // number 12 - index -1

        // the second parameter the index of the variable x on the Lua stack
        // gets the value of the third element stacked from the top
        lua_Number y = lua_tonumber(L, -3);
        printf("lua_Number y: %d\n", (int) y); // 11

        lua_remove(L, 2); // this should remove 11 from the stack
        lua_Number z = lua_tonumber(L, 2);
        printf("lua_Number z: %d\n", (int) z); // 12

        lua_close(L);
    }

    {
        lua_State* L = luaL_newstate();

//        constexpr char* lambdaFunction = R"(
//        Return4 = function()
//            return 4
//        end
//        )";

        // Lua code. Define function
        constexpr char* LUA_FILE = R"(
        function Return4()
            return 4
        end
        )";

        // execute a Lua string, second argument is the Lua code that will be executed
        luaL_dostring(L, LUA_FILE);
        // accesses the  function "Return4" in the Lua environment. The function is placed on the Lua stack.
        lua_getglobal(L, "Return4");
        // checks if the value at the top of the Lua stack is a Lua function
        if (lua_isfunction(L, -1)) {
            // calls the Lua stack function
            // (L, number inputs, number outputs, number of error handlers)
            lua_pcall(L, 0, 1, 0);
            // gets the value from the top of the Lua stack that was returned by the function
            lua_Number x = lua_tonumber(L, -1);
            printf("result function %d\n", (int)x);
        }

        lua_close(L);
    }

    {
        lua_State* L = luaL_newstate();

        // Lua code. Define function
        constexpr char* LUA_FILE = R"(
        function Pythagoras(a, b)
            return (a * a) + (b *b ), a, b
        end
        )";

        // execute a Lua string, second argument is the Lua code that will be executed
        luaL_dostring(L, LUA_FILE);
        // accesses the  function "Return4" in the Lua environment. The function is placed on the Lua stack.
        lua_getglobal(L, "Pythagoras");
        // checks if the value at the top of the Lua stack is a Lua function
        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, 3);
            lua_pushnumber(L, 4);

            constexpr int NUM_ARGS = 2;
            constexpr int NUM_RETURNS = 3;

            // calls the Lua stack function
            // (L, number inputs, number outputs, number of error handlers)
            lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);

            // push sequence
            // 3, 4, 25, 3, 4
            // Lua stack
            // 4, 3, 25, 4, 3
            // -1 acessa o topo da pilha

            // gets the value from the top of the Lua stack that was returned by the function
            lua_Number c = lua_tonumber(L, -3);
            lua_Number a = lua_tonumber(L, -2);
            lua_Number b = lua_tonumber(L, -1);
//            printf("csqr = %d\n", (int)c);
            printf("csqr(a = %d, b = %d) = %d\n", (int)a, (int)b, (int)c);
        }

        lua_close(L);
    }

    {
        lua_State* L = luaL_newstate();

        auto NativePythagoras = [](lua_State* L) -> int {
            lua_Number a = lua_tonumber(L, -2);
            lua_Number b = lua_tonumber(L, -1);
            lua_Number csqr = (a * a) + (b * b);
            lua_pushnumber(L, csqr);

            return 1;
        };

        // Lua code. Define function
        constexpr char* LUA_FILE = R"(
        function Pythagoras(a, b)
            csqr = NativePythagoras(a, b)
            return csqr, a, b
        end
        )";

        lua_pushcfunction(L, NativePythagoras);
        lua_setglobal(L, "NativePythagoras");
        // execute a Lua string, second argument is the Lua code that will be executed
        luaL_dostring(L, LUA_FILE);
        // accesses the  function "Return4" in the Lua environment. The function is placed on the Lua stack.
        lua_getglobal(L, "Pythagoras");
        // checks if the value at the top of the Lua stack is a Lua function
        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, 3);
            lua_pushnumber(L, 4);

            constexpr int NUM_ARGS = 2;
            constexpr int NUM_RETURNS = 3;

            // calls the Lua stack function
            // (L, number inputs, number outputs, number of error handlers)
            lua_pcall(L, NUM_ARGS, NUM_RETURNS, 0);

            // push sequence
            // 3, 4, 25, 3, 4
            // Lua stack
            // 4, 3, 25, 4, 3
            // -1 acessa o topo da pilha

            // gets the value from the top of the Lua stack that was returned by the function
            lua_Number c = lua_tonumber(L, -3);
            lua_Number a = lua_tonumber(L, -2);
            lua_Number b = lua_tonumber(L, -1);
//            printf("csqr = %d\n", (int)c);
            printf("NativeFunction: csqr(a = %d, b = %d) = %d\n", (int)a, (int)b, (int)c);
        }

        lua_close(L);
    }

    {
        struct Sprite {
            int x;
            int y;

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            sprite->x = 0;
            sprite->y = 0;
            return 1;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        constexpr char* LUA_FILE = R"(
        sprite = CreateSprite()
        MoveSprite(sprite, 7, 12)
        )";

        lua_State* L = luaL_newstate();
        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setglobal(L, "CreateSprite");
        lua_pushcfunction(L, MoveSprite);// Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setglobal(L, "MoveSprite");
        luaL_dostring(L, LUA_FILE); // inicializa o programa
        lua_getglobal(L, "sprite");
        if (lua_isuserdata(L, -1)) {
            printf("We got a sprite from lua\n");
            Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
            printf("Sprite x: %d, y: %d\n", sprite->x, sprite->y);
        }
        else {
            printf("We didnt got a sprite from lua\n");
        }

        lua_close(L);
    }

    // Tables
    printf("------- Tables ------------------ \n");
    {
        constexpr char *LUA_FILE = R"(
        x = {
            luan = "admin",
            marcelo = "user"
        }
        )";

        lua_State * L = luaL_newstate();
        luaL_dostring(L, LUA_FILE);

        // aponta para a variável global x (tabela)
        lua_getglobal(L, "x"); // 1 => -2
        // adiciona luan a pilha de acesso
        lua_pushstring(L, "luan"); // 2 => -1
        // seleciona o penultimo elemento da pilha (no caso a chave luan)
        lua_gettable(L, -2);
        assert(lua_isstring(L, -1));
        // converte o ultimo elemnto da pilha para string
        const char* luanIs = lua_tostring(L, -1);
        printf("Luan is %s\n", luanIs);

        // aponta para a variável global x (tabela)
        lua_getglobal(L, "x"); // 1 => -2
        // adiciona na pilha o ultimo valor de marcelo (que é a chave na tabela)
        lua_getfield(L, -1, "marcelo"); // 2 => -1
        // converte o ultimo valor da pilha em string (nesse caso o valor da chave marcelo user)
        const char* marceloIs = lua_tostring(L, -1);
        printf("Marcelo is %s\n", marceloIs);

        // aponta para a variável global x (tabela)
        lua_getglobal(L, "x"); // 1 => -2
        // adiciona a string editor na pilha
        lua_pushstring(L, "editor"); // 2 => -1
        // cria a chave jhon e adiciona o penultimo elemento da pilha "edito" como valor
        // essa função seria semelhante a x["jhon"] = "editor"
        lua_setfield(L, -2, "jhon");

        // aponta para a variável global x (tabela)
        lua_getglobal(L, "x"); // 1
        // adiciona na pilha o ultimo valor de jhon (que é a chave na tabela)
        lua_getfield(L, -1, "jhon"); // 2
        // converte o ultimo valor da pilha em string (nesse caso o valor da chave jhon user)
        const char* jhonIs = lua_tostring(L, -1);
        printf("Jhon is %s\n", jhonIs);

        lua_close(L);
    }

    printf("------- Metatables and Metamethods ------------------ \n");
    {
        struct Vec {
            static int CreateVector(lua_State* L) {
                lua_newtable(L); // 1 => -3
                lua_pushstring(L, "x"); // 2 => -2
                lua_pushnumber(L, 0); // 3 => -1
                lua_settable(L, -3);

                lua_pushstring(L, "y"); // 2 => -2
                lua_pushnumber(L, 0); // 3 => -1
                lua_settable(L, -3);

                luaL_getmetatable(L, "VectorMetaTable");
                lua_setmetatable(L, -2);

                return 1;
            }

            static int add(lua_State* L) {
                assert(lua_istable(L, -2)); // left table
                assert(lua_istable(L, -1)); // right table

                lua_pushstring(L, "x");
                lua_gettable(L, -3);
                lua_Number xLeft = lua_tonumber(L, -1);
                lua_pop(L, 1);

                lua_pushstring(L, "x");
                lua_gettable(L, -2);
                lua_Number xRight = lua_tonumber(L, -1);
                lua_pop(L, 1);

                lua_Number xAdded = xLeft + xRight;
                printf("xAdded = %d\n", (int)xAdded);

                Vec::CreateVector(L);

                lua_pushstring(L, "x");
                lua_pushnumber(L, xAdded);
                lua_rawset(L, -3);

                return 1;
            }
        };

        constexpr char* LUA_FILE = R"(
        v1 = CreateVector() -- v1 é uma tabela
        v2 = CreateVector() -- v2 é uma tabela
        v1.x = 5
        v2.x = 15
        v3 = v1 + v2
        result = v3.x
        )";
        lua_State* L = luaL_newstate();
        lua_pushcfunction(L, Vec::CreateVector);
        lua_setglobal(L, "CreateVector");
        luaL_newmetatable(L, "VectorMetaTable");
        lua_pushstring(L, "__add");
        lua_pushcfunction(L, Vec::add);
        lua_settable(L, -3);

        auto x = luaL_dostring(L, LUA_FILE);
        if (x != LUA_OK)
            printf("Error %s\n", lua_tostring(L, -1));

        lua_getglobal(L, "result");
        lua_Number result = lua_tonumber(L, -1);
        printf("Result = %d\n", (int)result);

        lua_close(L);
    }

    printf("------- Constructor and Destructor ------------------ \n");
    {
        static int numberOfSpriteExisting = 0;

        struct Sprite {
            int x;
            int y;

            Sprite() : x(0), y(0) {
                numberOfSpriteExisting++;
            }

            ~Sprite() {
                numberOfSpriteExisting--;
            }

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }

            void Draw() {
                printf("Sprite(%p): x = %d, y = %d\n", this, x, y);
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            void* sprite = lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            new (sprite) Sprite(); // 1 => -2
            luaL_getmetatable(L, "SpriteMetaTable"); // 2 => -1
            assert(lua_istable(L, -1));
            lua_setmetatable(L, -2);
            return 1;
        };

        auto DestroySprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->~Sprite();
            return 0;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        auto DrawSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->Draw();
            return 0;
        };

        constexpr char* LUA_FILE = R"(
        sprite = CreateSprite()
        MoveSprite(sprite, 7, 12)
        DrawSprite(sprite)
        MoveSprite(sprite, 20, 7)
        DrawSprite(sprite)

        sprite2 = CreateSprite()
        MoveSprite(sprite2, 45, 80)
        DrawSprite(sprite2)
        )";

        lua_State* L = luaL_newstate();

        luaL_newmetatable(L, "SpriteMetaTable"); // 1 => -3
        lua_pushstring(L, "__gc"); // 2 => -2
        lua_pushcfunction(L, DestroySprite); // 3 => -1
        lua_settable(L, -3);

        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setglobal(L, "CreateSprite");
        lua_pushcfunction(L, MoveSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setglobal(L, "MoveSprite");
        lua_pushcfunction(L, DrawSprite);
        lua_setglobal(L, "DrawSprite");

        luaL_dostring(L, LUA_FILE); // inicializa o programa
        lua_getglobal(L, "sprite");
        if (lua_isuserdata(L, -1)) {
            printf("We got a sprite from lua\n");
            Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
            printf("Sprite x: %d, y: %d\n", sprite->x, sprite->y);
        }
        else {
            printf("We didnt got a sprite from lua\n");
        }

        lua_close(L);

        assert(numberOfSpriteExisting == 0);
    }

    printf("-------------- Object Oriented Access ------------------ \n");
    {
        static int numberOfSpriteExisting = 0;

        struct Sprite {
            int x;
            int y;

            Sprite() : x(0), y(0) {
                numberOfSpriteExisting++;
            }

            ~Sprite() {
                numberOfSpriteExisting--;
            }

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }

            void Draw() {
                printf("Sprite(%p): x = %d, y = %d\n", this, x, y);
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            void* sprite = lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            new (sprite) Sprite(); // 1 => -2
            luaL_getmetatable(L, "SpriteMetaTable"); // 2 => -1
            assert(lua_istable(L, -1));
            lua_setmetatable(L, -2);
            return 1;
        };

        auto DestroySprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->~Sprite();
            return 0;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        auto DrawSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->Draw();
            return 0;
        };

        constexpr char* LUA_FILE = R"(
        sprite = Sprite.new()
        sprite:Move(7, 12) -- Sprite.Move(sprite, 7, 12)
        sprite:Draw()
        sprite:Move(20, 7)
        sprite:Draw()

        sprite2 = Sprite.new()
        sprite2:Move(45, 80)
        sprite2:Draw()

        -- sprite  -> sprite is a userdata
        --      has a metatable called SpriteMetaTable
        --          don't have Move(), use the __index metamethod
        --          __index metamethod is a table which is Sprite
        --          Sprite has a field called Move(), invoke that
        --          Move() is a c function
        --          invoke, pass the userdatum as the first parameter
        )";

        lua_State* L = luaL_newstate();

        lua_newtable(L);
        int spriteTableIdx = lua_gettop(L);
        lua_pushvalue(L, spriteTableIdx);
        lua_setglobal(L, "Sprite");

        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "new");
        lua_pushcfunction(L, MoveSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "Move");
        lua_pushcfunction(L, DrawSprite);
        lua_setfield(L, -2, "Draw");

        luaL_newmetatable(L, "SpriteMetaTable"); // 1 => -3
        lua_pushstring(L, "__gc"); // 2 => -2
        lua_pushcfunction(L, DestroySprite); // 3 => -1
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushvalue(L, spriteTableIdx);
        lua_settable(L, -3);

        luaL_dostring(L, LUA_FILE); // inicializa o programa
        lua_getglobal(L, "sprite");
        if (lua_isuserdata(L, -1)) {
            printf("We got a sprite from lua\n");
            Sprite* sprite = (Sprite*)lua_touserdata(L, -1);
            printf("Sprite x: %d, y: %d\n", sprite->x, sprite->y);
        }
        else {
            printf("We didnt got a sprite from lua\n");
        }

        lua_close(L);

        assert(numberOfSpriteExisting == 0);
    }

    printf("-------------- Reading Object Properties ------------------ \n");
    {
        static int numberOfSpriteExisting = 0;

        struct Sprite {
            int x;
            int y;

            Sprite() : x(0), y(0) {
                numberOfSpriteExisting++;
            }

            ~Sprite() {
                numberOfSpriteExisting--;
            }

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }

            void Draw() {
                printf("Sprite(%p): x = %d, y = %d\n", this, x, y);
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            void* sprite = lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            new (sprite) Sprite(); // 1 => -2
            luaL_getmetatable(L, "SpriteMetaTable"); // 2 => -1
            assert(lua_istable(L, -1));
            lua_setmetatable(L, -2);
            return 1;
        };

        auto DestroySprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->~Sprite();
            return 0;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        auto DrawSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->Draw();
            return 0;
        };

        auto SpriteIndex = [](lua_State* L) -> int {
            assert(lua_isuserdata(L, -2));
            assert(lua_isstring(L, -1));

            auto sprite = (Sprite*)lua_touserdata(L, -2); // criar um tipo criado pelo usuário
            const char* index = lua_tostring(L, -1);
            if (strcmp(index, "x") == 0) {
                lua_pushnumber(L, sprite->x);
            }
            else if (strcmp(index, "y") == 0) {
                lua_pushnumber(L, sprite->y);
            }
            else {
                lua_getglobal(L, "Sprite");
                lua_pushstring(L, index);
                lua_rawget(L, -2);
            }

            return 1;
        };

        constexpr char* LUA_FILE = R"(
        sprite = Sprite.new()
        sprite:Move(7, 12) -- Sprite.Move(sprite, 7, 12)
        sprite:Draw()

        temp_x = sprite.x
        )";

        lua_State* L = luaL_newstate();

        lua_newtable(L);
        int spriteTableIdx = lua_gettop(L);
        lua_pushvalue(L, spriteTableIdx);
        lua_setglobal(L, "Sprite");

        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "new");
        lua_pushcfunction(L, MoveSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "Move");
        lua_pushcfunction(L, DrawSprite);
        lua_setfield(L, -2, "Draw");

        luaL_newmetatable(L, "SpriteMetaTable"); // 1 => -3
        lua_pushstring(L, "__gc"); // 2 => -2
        lua_pushcfunction(L, DestroySprite); // 3 => -1
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
//        lua_pushvalue(L, spriteTableIdx);
        lua_pushcfunction(L, SpriteIndex);
        lua_settable(L, -3);

        int result = luaL_dostring(L, LUA_FILE); // inicializa o programa
        if (result != LUA_OK) {
            printf("Error %s\n", lua_tostring(L, -1));
        }

        lua_getglobal(L, "temp_x");
        lua_Number temp_x = lua_tonumber(L, -1);
        assert(temp_x == 7);

        lua_close(L);

        assert(numberOfSpriteExisting == 0);
    }

    printf("-------------- Writing Object Properties ------------------ \n");
    {
        static int numberOfSpriteExisting = 0;

        struct Sprite {
            int x;
            int y;

            Sprite() : x(0), y(0) {
                numberOfSpriteExisting++;
            }

            ~Sprite() {
                numberOfSpriteExisting--;
            }

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }

            void Draw() {
                printf("Sprite(%p): x = %d, y = %d\n", this, x, y);
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            void* sprite = lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            new (sprite) Sprite(); // 1 => -2
            luaL_getmetatable(L, "SpriteMetaTable"); // 2 => -1
            assert(lua_istable(L, -1));
            lua_setmetatable(L, -2);
            return 1;
        };

        auto DestroySprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->~Sprite();
            return 0;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        auto DrawSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->Draw();
            return 0;
        };

        auto SpriteIndex = [](lua_State* L) -> int {
            assert(lua_isuserdata(L, -2));
            assert(lua_isstring(L, -1));

            auto sprite = (Sprite*)lua_touserdata(L, -2); // criar um tipo criado pelo usuário
            const char* index = lua_tostring(L, -1);
            if (strcmp(index, "x") == 0) {
                lua_pushnumber(L, sprite->x);
            }
            else if (strcmp(index, "y") == 0) {
                lua_pushnumber(L, sprite->y);
            }
            else {
                lua_getglobal(L, "Sprite");
                lua_pushstring(L, index);
                lua_rawget(L, -2);
            }

            return 1;
        };

        auto SpriteNewIndex = [](lua_State* L) -> int {
            assert(lua_isuserdata(L, -3));
            assert(lua_isstring(L, -2));
            // -1 - value we want to set

            auto sprite = (Sprite*)lua_touserdata(L, -3); // criar um tipo criado pelo usuário
            const char* index = lua_tostring(L, -2);
            if (strcmp(index, "x") == 0) {
                sprite->x = (int)lua_tonumber(L, -1);
            }
            else if (strcmp(index, "y") == 0) {
                sprite->y = (int)lua_tonumber(L, -1);
            }
            else {
                assert(false); // dont't want you to write to my native object
            }

            return 0;
        };

        constexpr char* LUA_FILE = R"(
        sprite = Sprite.new()
        sprite:Move(7, 12) -- Sprite.Move(sprite, 7, 12)
        sprite:Draw()
        sprite.y = 23
        sprite:Draw()
        sprite.y = sprite.x
        sprite:Draw()
        temp_x = sprite.x
        )";

        lua_State* L = luaL_newstate();

        lua_newtable(L);
        int spriteTableIdx = lua_gettop(L);
        lua_pushvalue(L, spriteTableIdx);
        lua_setglobal(L, "Sprite");

        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "new");
        lua_pushcfunction(L, MoveSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "Move");
        lua_pushcfunction(L, DrawSprite);
        lua_setfield(L, -2, "Draw");

        luaL_newmetatable(L, "SpriteMetaTable"); // 1 => -3
        lua_pushstring(L, "__gc"); // 2 => -2
        lua_pushcfunction(L, DestroySprite); // 3 => -1
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, SpriteIndex);
        lua_settable(L, -3);

        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, SpriteNewIndex);
        lua_settable(L, -3);

        int result = luaL_dostring(L, LUA_FILE); // inicializa o programa
        if (result != LUA_OK) {
            printf("Error %s\n", lua_tostring(L, -1));
        }

        lua_getglobal(L, "temp_x");
        lua_Number temp_x = lua_tonumber(L, -1);
        assert(temp_x == 7);

        lua_close(L);

        assert(numberOfSpriteExisting == 0);
    }

    printf("-------------- User values ------------------ \n");
    {
        static int numberOfSpriteExisting = 0;

        struct Sprite {
            int x;
            int y;

            Sprite() : x(0), y(0) {
                numberOfSpriteExisting++;
            }

            ~Sprite() {
                numberOfSpriteExisting--;
            }

            void Move(int pX, int pY) {
                x += pX;
                y += pY;
            }

            void Draw() {
                printf("Sprite(%p): x = %d, y = %d\n", this, x, y);
            }
        };

        // Callback que será chamado no contexto do lua
        auto CreateSprite = [](lua_State* L) -> int {
            void* sprite = lua_newuserdata(L, sizeof(Sprite)); // criar um tipo criado pelo usuário
            new (sprite) Sprite(); // 1 => -2
            luaL_getmetatable(L, "SpriteMetaTable"); // 2 => -1
            assert(lua_istable(L, -1));
            lua_setmetatable(L, -2);

            lua_newtable(L);
            lua_setuservalue(L, 1);

            return 1;
        };

        auto DestroySprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->~Sprite();
            return 0;
        };

        // Callback que será chamado no contexto do lua
        auto MoveSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -3); // recupera o dado criado pelo id
            lua_Number velX = lua_tonumber(L, -2); // pega o parâmetro (pelo id) que será passado na chamada da função
            lua_Number velY = lua_tonumber(L, -1); // pega o parâmetro (pelo id) que será passado na chamada da função
            sprite->Move((int)velX, (int)velY);
            return 0;
        };

        auto DrawSprite = [](lua_State* L) -> int {
            auto sprite = (Sprite*)lua_touserdata(L, -1); // criar um tipo criado pelo usuário
            sprite->Draw();
            return 0;
        };

        auto SpriteIndex = [](lua_State* L) -> int {
            assert(lua_isuserdata(L, -2));
            assert(lua_isstring(L, -1));

            auto sprite = (Sprite*)lua_touserdata(L, -2); // criar um tipo criado pelo usuário
            const char* index = lua_tostring(L, -1);
            if (strcmp(index, "x") == 0) {
                lua_pushnumber(L, sprite->x);
            }
            else if (strcmp(index, "y") == 0) {
                lua_pushnumber(L, sprite->y);
            }
            else {
                lua_getuservalue(L, 1);
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
                if (lua_isnil(L, -1)) {
                    lua_getglobal(L, "Sprite");
                    lua_pushstring(L, index);
                    lua_rawget(L, -2);
                }
            }

            return 1;
        };

        auto SpriteNewIndex = [](lua_State* L) -> int {
            assert(lua_isuserdata(L, -3)); // 1
            assert(lua_isstring(L, -2)); // 2
            // -1 - value we want to set // 3

            auto sprite = (Sprite*)lua_touserdata(L, -3); // criar um tipo criado pelo usuário
            const char* index = lua_tostring(L, -2);
            if (strcmp(index, "x") == 0) {
                sprite->x = (int)lua_tonumber(L, -1);
            }
            else if (strcmp(index, "y") == 0) {
                sprite->y = (int)lua_tonumber(L, -1);
            }
            else {
                lua_getuservalue(L, 1); // 1
                lua_pushvalue(L, 2); // 2
                lua_pushvalue(L, 3); // 3
                lua_settable(L, -3);
            }

            return 1;
        };

        constexpr char* LUA_FILE = R"(
        sprite = Sprite.new()
        sprite:Move(7, 12) -- Sprite.Move(sprite, 7, 12)
        sprite:Draw()
        sprite.y = 23
        sprite:Draw()
        sprite.y = sprite.x
        sprite:Draw()
        sprite.zzz = 99
        sprite.x = sprite.zzz
        sprite:Draw()
        temp_x = sprite.x
        )";

        lua_State* L = luaL_newstate();

        lua_newtable(L);
        int spriteTableIdx = lua_gettop(L);
        lua_pushvalue(L, spriteTableIdx);
        lua_setglobal(L, "Sprite");

        lua_pushcfunction(L, CreateSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "new");
        lua_pushcfunction(L, MoveSprite); // Registra o callback antes da inicialização do programa lua (LUA_FILE)
        lua_setfield(L, -2, "Move");
        lua_pushcfunction(L, DrawSprite);
        lua_setfield(L, -2, "Draw");

        luaL_newmetatable(L, "SpriteMetaTable"); // 1 => -3
        lua_pushstring(L, "__gc"); // 2 => -2
        lua_pushcfunction(L, DestroySprite); // 3 => -1
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, SpriteIndex);
        lua_settable(L, -3);

        lua_pushstring(L, "__newindex");
        lua_pushcfunction(L, SpriteNewIndex);
        lua_settable(L, -3);

        int result = luaL_dostring(L, LUA_FILE); // inicializa o programa
        if (result != LUA_OK) {
            printf("Error %s\n", lua_tostring(L, -1));
        }

        lua_getglobal(L, "temp_x");
        lua_Number temp_x = lua_tonumber(L, -1);
        assert(temp_x == 99);

        lua_close(L);

        assert(numberOfSpriteExisting == 0);
    }

    printf("-------------- Lua Arena Memory Allocation ------------------ \n");
    {
        struct ArenaAllocator {
            void* begin;
            void* end;
            char* curr;

            ArenaAllocator(void* pbegin, void* pend) :
            begin(pbegin),
            end(pend),
            curr(static_cast<char*>(begin))
            {
            }

            void* alloc(size_t size) {
                printf("Allocated %d bytes\n", (int)size);
                void* ptr = curr;
                curr += size;
                return ptr;
            }

            void deAlloc(void* /*ptr*/) {
                printf("DeAllocated ?? bytes\n");
                //free(ptr);
            }

            void* reAlloc(void* ptr, size_t oldSize, size_t nsize) {
                void* newPtr = alloc(nsize);
                memcpy(newPtr, ptr, oldSize);
                deAlloc(ptr);

                return newPtr;
            }

            static void* l_alloc(void* ud, void *ptr, size_t osize, size_t nsize) {
                ArenaAllocator* pool = static_cast<ArenaAllocator*>(ud);

                if (nsize == 0) {
                    pool->deAlloc(ptr);
                    return nullptr;
                }

                if (ptr == nullptr)
                    return pool->alloc(nsize);

                return pool->reAlloc(ptr, osize, nsize);
            }
        };

        constexpr int POOL_SIZE = 1024 * 20;
        char memory[POOL_SIZE];
        ArenaAllocator pool(memory, &memory[POOL_SIZE-1]);
        lua_State* L = lua_newstate(ArenaAllocator::l_alloc, &pool);
        assert(L != nullptr);
        lua_close(L);
    }

    return 0;
}
