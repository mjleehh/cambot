export enum YawDirection {
    left,
    right,
}

export enum PitchDirection {
    up,
    down,
}

export enum RollDirection {
    ccw,
    cw,
}

export type Rotation = {
    deg: number
    steps: number
}

export type Position = {
    x: number
    y: number
    z: number
}

export type Move = {
    forward: number
    horizontal: number
    vertical: number
}

export enum DirectionForward {
    forward,
    backward,
}

export enum DirectionHorizontal {
    left,
    right,
}

export enum DirectionVertical {
    up,
    down,
}

export type Rotations = {
    yaw: Rotation,
    lowerVert: Rotation,
    upperVert: Rotation,
    roll: Rotation,
    pitch: Rotation,
}

export type Angles = {
    theta: number,
    phi0: number,
    phi1: number,
    rho: number,
    phi2: number,
}

export type ArmState = {
    rotations: Rotations
    toolPosition: Position
    wristPosition: Position
    inverse: Angles
}
