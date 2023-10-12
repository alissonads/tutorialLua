#include <iostream>
#include <lua5.4/lua.hpp>

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

    return 0;
}
