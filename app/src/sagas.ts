import {
    RequestYawAction,
    REQUEST_YAW,
    REQUEST_UPPER_VERT,
    RequestUpperVertAction,
    REQUEST_LOWER_VERT,
    REQUEST_PITCH,
    REQUEST_ROLL,
    RequestRollAction,
    RequestPitchAction,
    SetHomeAction,
    gotArmState,
    SET_HOME,
    MOVE_YAW,
    GO_HOME,
    MoveToAction,
    MOVE_TO,
    REQUEST_STOP,
    RequestMoveForwardAction,
    RequestMoveHorizontalAction,
    RequestMoveVerticalAction, REQUEST_MOVE_FORWARD, REQUEST_MOVE_HORIZONTAL, REQUEST_MOVE_VERTICAL, REQUEST_ARM_STATE
} from './actions'
import {takeEvery, call, select, fork, delay, put} from 'redux-saga/effects'
import api from './api'

function* yawSaga({payload: direction}: RequestYawAction) {
    const {numSteps, rotationSpeed} = yield select()
    yield call([api, api.yaw], direction, numSteps, rotationSpeed)
}

function* lowerPitchSaga({payload: direction}: RequestUpperVertAction) {
    const {numSteps, rotationSpeed} = yield select()
    yield call([api, api.lowerVert], direction, numSteps, rotationSpeed)
}

function* upperPitchSaga({payload: direction}: RequestUpperVertAction) {
    const {numSteps, rotationSpeed} = yield select()
    yield call([api, api.upperVert], direction, numSteps, rotationSpeed)
}

function* rollSaga({payload: direction}: RequestRollAction) {
    const {numSteps, rotationSpeed} = yield select()
    yield call([api, api.roll], direction, numSteps, rotationSpeed)
}

function* pitchSaga({payload: direction}: RequestPitchAction) {
    const {numSteps, rotationSpeed} = yield select()
    yield call([api, api.pitch], direction, numSteps, rotationSpeed)
}

function* moveFowardSaga({payload: direction}: RequestMoveForwardAction) {
    const {distance, speed} = yield select()
    try {
        yield call([api, api.moveForward], direction, distance, speed)
    } catch (e) {
        console.log('failed to move forward')
    }
}

function* moveHorizontalSaga({payload: direction}: RequestMoveHorizontalAction) {
    const {distance, speed} = yield select()
    try {
        yield call([api, api.moveHorizontal], direction, distance, speed)
    } catch (e) {
        console.log('failed to move horizontally')
    }
}

function* moveVerticalSaga({payload: direction}: RequestMoveVerticalAction) {
    const {distance, speed} = yield select()
    try {
        yield call([api, api.moveVertical], direction, distance, speed)
    } catch (e) {
        console.log('failed to move vertically')
    }
}

function* setHomeSaga() {
    yield call([api, api.setHome])
}

function* goHomeSaga() {
    yield call([api, api.goHome], 500)
}

function* pollArmStateSaga(): any{
    while (true) {
        try {
            const armState = yield call([api, api.armState])
            yield put(gotArmState(armState))
        } catch (e) {
            console.log('polling positions failed')
        }
        yield delay(1000)
    }
}

function* requestArmStateSaga(): any {
    try {
        const armState = yield call([api, api.armState])
        yield put(gotArmState(armState))
    } catch (e) {
        console.log('failed to fetch arm state')
    }
}

function* moveYawSaga({payload: angle}: RequestRollAction) {
    yield call([api, api.moveYaw], angle, 1)
}

function* moveToSaga({payload: {x, y, z, speed}}: MoveToAction) {
    yield call([api, api.moveTo], x, y, z, speed)
}

function* requestStopSaga() {
    try {
        yield call([api, api.stop])
    } catch(e) {
        console.log('failed to stop robot')
    }
}

export default function* rootSaga() {
    yield takeEvery(REQUEST_STOP, requestStopSaga)
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_VERT, upperPitchSaga)
    yield takeEvery(REQUEST_LOWER_VERT, lowerPitchSaga)
    yield takeEvery(REQUEST_ROLL, rollSaga)
    yield takeEvery(REQUEST_PITCH, pitchSaga)
    yield takeEvery(SET_HOME, setHomeSaga)
    yield takeEvery(GO_HOME, goHomeSaga)
    yield takeEvery(MOVE_YAW, moveYawSaga)
    yield takeEvery(MOVE_TO, moveToSaga)
    yield takeEvery(REQUEST_MOVE_FORWARD, moveFowardSaga)
    yield takeEvery(REQUEST_MOVE_HORIZONTAL, moveHorizontalSaga)
    yield takeEvery(REQUEST_MOVE_VERTICAL, moveVerticalSaga)
    yield takeEvery(REQUEST_ARM_STATE, requestArmStateSaga)
    //yield fork(pollArmStateSaga)
}