import createSagaMiddleware from 'redux-saga'
import {applyMiddleware, createStore} from 'redux'
import reducer from './reducer'
import {composeWithDevTools} from 'redux-devtools-extension'
import rotateRobotSaga from './sagas'
import {Action} from './actions'

const sagaMiddleware = createSagaMiddleware()
export const store = createStore(reducer, composeWithDevTools(applyMiddleware(sagaMiddleware)))
sagaMiddleware.run(rotateRobotSaga)

export type Dispatch = (action: Action) => void
export const dispatch: Dispatch = (action: Action) => store.dispatch(action)
