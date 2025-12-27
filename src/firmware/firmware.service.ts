import * as fs from 'fs';
import * as path from 'path';
import { Injectable } from '@nestjs/common';

@Injectable()
export class FirmwareService {

  private readonly TEMPLATE_DIR = path.join(
    process.cwd(),
    'src',
    'firmware',
    'templates',
  );

  generate(sensors: string[]) {

    const basePath = path.join(this.TEMPLATE_DIR, 'base.ino');

    if (!fs.existsSync(basePath)) {
      throw new Error(`Base template not found at ${basePath}`);
    }

    let base = fs.readFileSync(basePath, 'utf8');

    let includes = '';
    let globals = '';
    let setup = '';
    let loop = '';

    sensors.forEach(sensor => {
      const sensorPath = path.join(this.TEMPLATE_DIR, `${sensor}.ino`);

      if (!fs.existsSync(sensorPath)) {
        throw new Error(`Sensor template not found: ${sensor}.ino`);
      }

      const code = fs.readFileSync(sensorPath, 'utf8');

      includes += `\n// ==== ${sensor.toUpperCase()} ====\n`;
      includes += (code.match(/#include[^\n]*/g) || []).join('\n') + '\n';

      globals += code.replace(/#include[^\n]*/g, '') + '\n';

      const name = sensor.charAt(0).toUpperCase() + sensor.slice(1);
      setup += `setup${name}();\n`;
      loop  += `loop${name}();\n`;
    });

    return base
      .replace('{{INCLUDES}}', includes.trim())
      .replace('{{GLOBALS}}', globals.trim())
      .replace('{{SETUP_CODE}}', setup.trim())
      .replace('{{LOOP_CODE}}', loop.trim());
  }
}
