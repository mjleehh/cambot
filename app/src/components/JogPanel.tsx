import {
    MOVE_BACKWARD_KEY,
    MOVE_DOWN_KEY, MOVE_FORWARD_KEY, MOVE_LEFT_KEY, MOVE_RIGHT_KEY, MOVE_UP_KEY, STOP_KEY,
} from '../key-bindings'
import {Button} from 'antd'
import {
    requestMoveForward,
    requestMoveHorizontal,
    requestMoveVertical,
} from '../actions'
import React from 'react'
import {useDispatch} from 'react-redux'
import {DirectionForward, DirectionHorizontal, DirectionVertical, PitchDirection, YawDirection} from '../types'
import {
    CaretDownOutlined,
    CaretUpOutlined,
    DownOutlined,
    HomeOutlined,
    LeftOutlined,
    RightOutlined,
    UpOutlined
} from '@ant-design/icons'

const style: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'row',
    alignItems: 'center',
}

const directionStyle: React.CSSProperties = {
    display: 'grid',
    gridTemplateColumns: '9em 9em 9em',
    gridAutoRows: '9em',
    margin: '1em',
    gap: '0.3em',
}

const upDownStyle: React.CSSProperties = {
    display: 'grid',
    gridTemplateColumns: '9em',
    gridAutoRows: '9em',
    margin: '1em',
    gap: '0.3em'
}

const buttonStyle: React.CSSProperties = {
    height: '100%',
    width: '100%',
}

const JogPannel: React.FunctionComponent = () => {
    const dispatch = useDispatch()

    return <div style={style}>
        <div style={directionStyle}>
            <div> </div>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveForward(DirectionForward.forward))}><UpOutlined /> ({MOVE_FORWARD_KEY})</Button>
            <div> </div>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveHorizontal(DirectionHorizontal.left))}><LeftOutlined /> ({MOVE_LEFT_KEY})</Button>
            <Button style={buttonStyle} onClick={() => console.log('hahaha nothing happening :p')}><HomeOutlined /> ({STOP_KEY})</Button>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveHorizontal(DirectionHorizontal.right))}><RightOutlined /> ({MOVE_RIGHT_KEY})</Button>
            <div> </div>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveForward(DirectionForward.backward))}><DownOutlined /> ({MOVE_BACKWARD_KEY})</Button>
            <div> </div>
        </div>
        <div style={upDownStyle}>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveVertical(DirectionVertical.up))}><CaretUpOutlined /> ({MOVE_UP_KEY})</Button>
            <Button style={buttonStyle} onClick={() => dispatch(requestMoveVertical(DirectionVertical.down))}><CaretDownOutlined /> ({MOVE_DOWN_KEY})</Button>
        </div>
    </div>
}
JogPannel.displayName = 'JogPannel'
export default JogPannel