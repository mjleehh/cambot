import {PITCH_DOWN_KEY, PITCH_UP_KEY, ROLL_LEFT_KEY, ROLL_RIGHT_KEY} from '../key-bindings'
import {Button} from 'antd'
import {requestPitch, requestRoll} from '../actions'
import React from 'react'
import {useDispatch} from 'react-redux'
import {PitchDirection, RollDirection} from '../types'
import {DownOutlined, RedoOutlined, UndoOutlined, UpOutlined} from '@ant-design/icons'

const style: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    margin: '1em',
    gap: '0.3em',
}

const buttonStyle: React.CSSProperties = {
    height: '9em',
    width: '9em',
}

const horizontalStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'row',
    gap: '0.3em'
}

const StepHeadButtons: React.FunctionComponent = () => {
    const dispatch = useDispatch()

    return <div style={style}>
        <Button style={buttonStyle} onClick={() => dispatch(requestPitch(PitchDirection.up))}><UpOutlined /> ({PITCH_UP_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestPitch(PitchDirection.down))}><DownOutlined /> ({PITCH_DOWN_KEY})</Button>
        <div style={horizontalStyle}>
            <Button style={buttonStyle} onClick={() => dispatch(requestRoll(RollDirection.ccw))}><UndoOutlined /> ({ROLL_LEFT_KEY})</Button>
            <Button style={buttonStyle} onClick={() => dispatch(requestRoll(RollDirection.cw))}><RedoOutlined /> ({ROLL_RIGHT_KEY})</Button>
        </div>
    </div>
}
StepHeadButtons.displayName = 'StepHeadButtons'
export default StepHeadButtons