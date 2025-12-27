import * as fs from 'fs';
import * as path from 'path';
import { Injectable } from '@nestjs/common';

@Injectable()
export class FirmwareService {

    private readonly TEMPLATE_DIR = fs.existsSync(
        path.join(process.cwd(), 'dist', 'firmware', 'templates')
    )
        ? path.join(process.cwd(), 'dist', 'firmware', 'templates')
        : path.join(process.cwd(), 'src', 'firmware', 'templates');

    private readonly SENSOR_FUNC_MAP: Record<string, string> = {
        weather: 'Weather',
        soil: 'Soil',
        npk: 'NPK',
        gps: 'GPS',
    };


    generate(sensors: string[]): string {
        
        const basePath = path.join(this.TEMPLATE_DIR, 'base.ino');

        if (!fs.existsSync(basePath)) {
            throw new Error(`Base template not found at ${basePath}`);
        }

        let base = fs.readFileSync(basePath, 'utf8');

        let includes = '';
        let globals = '';
        let setup = '';
        let loop = '';

        for (const sensor of sensors) {

            const funcName = this.SENSOR_FUNC_MAP[sensor];
            if (!funcName) {
                throw new Error(`Unsupported sensor: ${sensor}`);
            }

            const sensorPath = path.join(this.TEMPLATE_DIR, `${sensor}.ino`);
            if (!fs.existsSync(sensorPath)) {
                throw new Error(`Sensor template not found: ${sensor}.ino`);
            }

            const code = fs.readFileSync(sensorPath, 'utf8');

            // Includes
            includes += `\n// ==== ${sensor.toUpperCase()} ====\n`;
            includes += (code.match(/#include[^\n]*/g) || []).join('\n') + '\n';

            // Globals (remove includes)
            globals += code.replace(/#include[^\n]*/g, '').trim() + '\n\n';

            // Setup / Loop
            setup += `setup${funcName}();\n`;
            loop += `loop${funcName}();\n`;
        }

        return base
            .replace('{{INCLUDES}}', includes.trim())
            .replace('{{GLOBALS}}', globals.trim())
            .replace('{{SETUP_CODE}}', setup.trim())
            .replace('{{LOOP_CODE}}', loop.trim());
    }
}
