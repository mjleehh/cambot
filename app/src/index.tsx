import React from 'react'
import ReactDom from 'react-dom'
import {Provider} from 'react-redux'
import {applyMiddleware, createStore} from 'redux'
import {composeWithDevTools} from 'redux-devtools-extension'
import createSagaMiddleware from 'redux-saga'
import App from './App'
import Mousetrap from 'mousetrap'
import {Action, requestLowerPitch, requestPitch, requestRoll, requestUpperPitch, requestYaw} from './actions'
import rotateRobotSaga from './sagas'
import {PitchDirection, RollDirection, YawDirection} from './rotation-types'

function reducer(state = {}, action: Action) {
    return state
}

const sagaMiddleware = createSagaMiddleware()
const store = createStore(reducer, composeWithDevTools(applyMiddleware(sagaMiddleware)))
sagaMiddleware.run(rotateRobotSaga)

Mousetrap.bind('left', () =>
    store.dispatch(requestYaw(YawDirection.left, 200, 300)))

Mousetrap.bind('right', () =>
    store.dispatch(requestYaw(YawDirection.right, 200, 300)))

Mousetrap.bind('pageup', () =>
    store.dispatch(requestUpperPitch(PitchDirection.up, 1000, 500)))

Mousetrap.bind('pagedown', () =>
    store.dispatch(requestUpperPitch(PitchDirection.down, 1000, 500)))

Mousetrap.bind('up', () =>
    store.dispatch(requestLowerPitch(PitchDirection.up, 1000, 500)))

Mousetrap.bind('down', () =>
    store.dispatch(requestLowerPitch(PitchDirection.down, 1000, 500)))

Mousetrap.bind('a', () =>
    store.dispatch(requestRoll(RollDirection.ccw, 200, 500)))

Mousetrap.bind('d', () =>
    store.dispatch(requestRoll(RollDirection.cw, 200, 500)))

Mousetrap.bind('w', () =>
    store.dispatch(requestPitch(PitchDirection.up, 1000, 500)))

Mousetrap.bind('s', () =>
    store.dispatch(requestPitch(PitchDirection.down, 1000, 500)))

ReactDom.render(
    <Provider store={store}>
        <App/>
    </Provider>,
    document.getElementById('main')
)
