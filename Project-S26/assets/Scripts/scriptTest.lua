
function start(self, ent)
    self.timer = 0

    self.emitter = ent:get_particle_emitter()

    self.emitter:set_particle_velocity( -100, -100, 100, -100);
    self.emitter:set_particle_rotation(1, 2)
    self.emitter:set_particle_scale(0.05, 0.1)
    self.emitter:set_particle_lifetime(10, 15)
end

function update(self, ent, dt)
    if(input:state():mouse():left():just_pressed()) then self.emitter:emit(5) end

    self.t = ent:get_transform()
    self.t:set_x(input:state():mouse():x())
    self.t:set_y(input:state():mouse():y())
end