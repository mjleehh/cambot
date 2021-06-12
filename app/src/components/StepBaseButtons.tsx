import {
    LOWER_VERT_DOWN_KEY,
    LOWER_VERT_UP_KEY,
    UPPER_VERT_DOWN_KEY,
    UPPER_VERT_UP_KEY,
    YAW_LEFT_KEY,
    YAW_RIGHT_KEY
} from '../key-bindings'
import {Button} from 'antd'
import {requestLowerVert, requestUpperVert, requestYaw} from '../actions'
import React from 'react'
import {useDispatch} from 'react-redux'
import {PitchDirection, YawDirection} from '../types'
import {DownOutlined, LeftOutlined, RightOutlined, UpOutlined} from '@ant-design/icons'

const style: React.CSSProperties = {
    display: 'grid',
    gridTemplateColumns: '9em 9em',
    gridAutoRows: '9em',
    margin: '1em',
    gap: '0.3em',
}

const buttonStyle: React.CSSProperties = {
    height: '100%',
    width: '100%',
}

const StepBaseButtons: React.FunctionComponent = () => {
    const dispatch = useDispatch()

    return <div style={style}>
        <Button style={buttonStyle} onClick={() => dispatch(requestLowerVert(PitchDirection.up))}><UpOutlined /> ({LOWER_VERT_UP_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestUpperVert(PitchDirection.up))}><UpOutlined /> ({UPPER_VERT_UP_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestLowerVert(PitchDirection.down))}><DownOutlined /> ({LOWER_VERT_DOWN_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestUpperVert(PitchDirection.down))}><DownOutlined /> ({UPPER_VERT_DOWN_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestYaw(YawDirection.left))}><LeftOutlined /> ({YAW_LEFT_KEY})</Button>
        <Button style={buttonStyle} onClick={() => dispatch(requestYaw(YawDirection.right))}><RightOutlined /> ({YAW_RIGHT_KEY})</Button>
    </div>
}
StepBaseButtons.displayName = 'StepBaseButtons'
export default StepBaseButtons