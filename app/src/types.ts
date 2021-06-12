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

export type Rotations = {
    yaw: Rotation,
    lowerVert: Rotation,
    upperVert: Rotation,
    roll: Rotation,
    pitch: Rotation,
}
