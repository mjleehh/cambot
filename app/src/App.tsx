import React from 'react'
import {Button, Radio, Slider} from 'antd'
import {useDispatch, useSelector} from 'react-redux'
import {
    requestLowerPitch,
    requestPitch,
    requestRoll,
    requestUpperPitch,
    requestYaw,
    setNumSteps,
    setSpeed
} from './actions'
import {State} from './reducer'
import {DownOutlined, LeftOutlined, RedoOutlined, RightOutlined, UndoOutlined, UpOutlined} from '@ant-design/icons'
import {PitchDirection, RollDirection, YawDirection} from './rotation-types'

const appStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
}

const numStepButtonStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'row',
}

const arrowsStyle = {
    display: 'grid',
    gridTemplateColumns: '8em 8em',
    columnGap: '1em',
    rowGap: '1em',
}

const numStepsOptions = [
    {label: '1 step', value: 1},
    {label: '10 steps', value: 10},
    {label: '100 steps', value: 100},
    {label: '1000 steps', value: 1000},
]

const App = () => {
    const dispatch = useDispatch()
    const numSteps = useSelector<State>(({numSteps}) => numSteps)
    const speed = useSelector<State>(({speed}) => speed)

    return <div style={appStyle}>
        <div style={numStepButtonStyle}>
            <Radio.Group optionType='button' options={numStepsOptions} value={numSteps} onChange={e => dispatch(setNumSteps(e.target.value))}/>
        </div>
        <div style={{display: 'flex', flexDirection: 'row'}}>
            <Slider style={{height: '12em'}} vertical min={10} max={1000} defaultValue={speed} onChange={(value: number) => dispatch(setSpeed(value))}/>
            <div style={arrowsStyle}>
                <Button onClick={() => dispatch(requestLowerPitch(PitchDirection.up))}><UpOutlined /></Button>
                <Button onClick={() => dispatch(requestUpperPitch(PitchDirection.up))}><UpOutlined /></Button>
                <Button onClick={() => dispatch(requestLowerPitch(PitchDirection.down))}><DownOutlined /></Button>
                <Button onClick={() => dispatch(requestUpperPitch(PitchDirection.down))}><DownOutlined /></Button>
                <Button onClick={() => dispatch(requestYaw(YawDirection.left))}><LeftOutlined /></Button>
                <Button onClick={() => dispatch(requestYaw(YawDirection.right))}><RightOutlined /></Button>
            </div>

            <div>
                <Button onClick={() => dispatch(requestPitch(PitchDirection.up))}><UpOutlined /></Button>
                <Button onClick={() => dispatch(requestPitch(PitchDirection.down))}><UpOutlined /></Button>
                <Button onClick={() => dispatch(requestRoll(RollDirection.ccw))}><UndoOutlined /></Button>
                <Button onClick={() => dispatch(requestRoll(RollDirection.cw))}><RedoOutlined /></Button>
            </div>
        </div>
    </div>
}
App.displayName = 'App'
export default App