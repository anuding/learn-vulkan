# Application
it holds:
- instance
- device
- swapchain
- sync objects
- pipelines
- queues
1. currently, one instance and one device
2. predefined some pipelines
3. predefined some queues

## Game extends from Application
- scenes
- input

### Scene
- scene tree
- camera
- objects


# Editor(separate from Application)
- import Game
- GUI

1. Editor will be an empty gui framework .
2. Editor loads a game instance and edit the scene data.
3. Scene data will be read from a json(or other format) file(init).
4. Editor should handle mouse and keyboard event from Game


# Script System
1. C++ code calls every object's update. And this is actually calls update function of this object's script.
    ``` c++
    // part of game loading, attach scripts to objs
    for(auto & obj:gameObjects) {
        lua_State *L = luaL_newState();;
        obj.luaState = L;
        luaL_openLibs(L);
        lua_register(L, "func1", func1);
        lua_register(L, "func2", func2);
        lua_register(L, "func3", func3);
        //...
        luaL_loadfile(L, obj.scriptPath)
    }

    //start
    for(auto & obj:gameObjects) {
        lua_State *L = obj.luaState;
        luaL_getfield(L,LUA_REGISTERYINDEX, "start");
        lua_pcall();
    }

    //update
    for(auto & obj:gameObjects) {
        lua_State *L = obj.luaState;
        luaL_getfield(L,LUA_REGISTERYINDEX, "update");
        lua_pcall();
    }
    ```
2. scene
    ``` lua
    -- scene.lua
    function start()
        -- do sth related to init obj's attribute
    end

    function update()
        -- update obj, mainly gameplay definition
    end
    ```


