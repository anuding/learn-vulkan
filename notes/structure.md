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