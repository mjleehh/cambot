import React from 'react'
import ReactDom from 'react-dom'
import {Provider} from 'react-redux'
import {applyMiddleware, createStore} from 'redux'
import {composeWithDevTools} from 'redux-devtools-extension'
import createSagaMiddleware from 'redux-saga'
import App from './App'
import Mousetrap from 'mousetrap'
import {Action, requestUpperPitch, requestYaw} from './actions'
import rotateRobotSaga from './sagas'
import {PitchDirection, YawDirection} from './rotation-types'

function reducer(state = {}, action: Action) {
    return state
}

const sagaMiddleware = createSagaMiddleware()
const store = createStore(reducer, composeWithDevTools(applyMiddleware(sagaMiddleware)))
sagaMiddleware.run(rotateRobotSaga)

Mousetrap.bind('a', () =>
    store.dispatch(requestYaw(YawDirection.cw, 200, 300)))

Mousetrap.bind('d', () =>
    store.dispatch(requestYaw(YawDirection.ccw, 200, 300)))

Mousetrap.bind('w', () =>
    store.dispatch(requestUpperPitch(PitchDirection.up, 1000, 500)))

Mousetrap.bind('s', () =>
    store.dispatch(requestUpperPitch(PitchDirection.down, 1000, 500)))

ReactDom.render(
    <Provider store={store}>
        <App/>
    </Provider>,
    document.getElementById('main')
)
